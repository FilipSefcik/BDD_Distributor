#pragma once
#include "module_manager.h"
#include "divider.h"
#include "mpi_manager.h"

class process {
protected:
    mpi_manager mpi_manager_;
    std::string my_instructions;
    int my_rank, assigned_count;
public:
    process(int rank) : my_rank(rank) {};
    virtual void process_information() = 0;
    void process_instructions(int state) {
        this->mpi_manager_.write_to_pla();
        this->mpi_manager_.complete_instructions(this->my_instructions, state);
    };
};

class main_process : public process {
private:
    std::vector<int> assigned_modules;  
    int used_count = 0; 
    int process_count = 0;
    module_manager module_manager_;
    divider* divider_;
    std::string conf_path;

public:
    main_process(int rank, int pa_process_count) : process(rank), process_count(pa_process_count) {};

    ~main_process() { if (divider_) { delete divider_; } };

    void set_conf_path(std::string path) { this->conf_path = path; };

    void process_information() override {
        this->assigned_modules.resize(this->process_count);

	    this->module_manager_.load(this->conf_path);

	    this->divider_->divide_modules(this->module_manager_.get_modules(), &this->assigned_modules);  

        MPI_Scatter(this->assigned_modules.data(), 1, MPI_INT, &this->assigned_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        this->get_used_processes_count();

        this->module_manager_.get_instructions(used_count);

        this->distribute_modules();
    }

    void get_used_processes_count() {
        for (int i = 0; i < this->assigned_modules.size(); i++) {
            if (this->assigned_modules.at(i) > 0) {
                this->used_count++;
            } else {
                // may be changed depending on what divider will be used
                break;
            }
        }
    }

    void distribute_modules() {
        //send to each used process
        for (int i = 0; i < this->used_count; i++) {
        //send each module assigned
        std::vector<module*>* nodes_modules = this->module_manager_.get_modules_for_process(i);
            for (int j = 0; j < nodes_modules->size(); j++) {
                module* mod = nodes_modules->at(j);

                if (i == this->my_rank) {
                    this->mpi_manager_.add_new_module(mod->get_name(), mod->get_pla(), this->my_rank, mod->get_var_count(), mod->get_function_column());
                    this->my_instructions = this->module_manager_.get_instructions_for_process(i);
                    continue;
                }

                this->mpi_manager_.send_module_info(mod, this->module_manager_.get_instructions_for_process(i), i);
            }
            delete nodes_modules;
        }
    }

    void set_divider(int flag) {
        switch (flag) {
            case 0:
                divider_ = new var_count_divider();
                break;
            default:
                divider_ = new node_divider();
                break;
        }
    }
};

class slave_process : public process {
public:
    slave_process(int rank) : process(rank) {}
    void process_information() override {
        MPI_Scatter(nullptr, 1, MPI_INT, &this->assigned_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (this->assigned_count > 0) {
            this->mpi_manager_.recieve_my_modules(this->assigned_count, this->my_rank, this->my_instructions);
        }
    }
};
