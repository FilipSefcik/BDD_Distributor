#pragma once
#include "module_manager.h"
#include "divider.h"
#include "mpi_communicator.h"
#include "mpi_manager.h"
#include <cstddef>
#include "merger.h"

/*
* @brief described basic functions of any process
*/
class process {
protected:
    mpi_manager mpi_manager_;
    std::string my_instructions;
    int my_rank, assigned_count;
public:
    process(int rank) : my_rank(rank) {};
    virtual void process_information() = 0;
    void process_instructions(int state) {
        // this->mpi_manager_.write_to_pla();
        // this->mpi_manager_.complete_instructions(this->my_instructions, state);
    };
};

/*
* @brief main_process's job is to load, divide and distribute modules from config file
*/
class main_process : public process {
private:
    int used_count = 0; 
    int process_count = 0;
    module_manager module_manager_;
    divider* divider_;
    std::string conf_path;
    std::vector<int> assigned_modules_count;  
    std::vector<module*>* assigned_modules;

public:
    main_process(int rank, int pa_process_count) : process(rank), process_count(pa_process_count) {};

    ~main_process() { if (divider_) { delete divider_;  delete this->assigned_modules; } };

    void set_conf_path(std::string path) { this->conf_path = path; };

    /*
    * @brief loads, assigns and distribute modules
    */
    void process_information() override {
        this->assigned_modules_count.resize(this->process_count);
        this->assigned_modules = new std::vector<module*>[this->process_count];

	    this->module_manager_.load(this->conf_path);

        merger merger;
        merger.set_modules(this->module_manager_.get_modules());
        merger.merge_pla();

	    // this->divider_->divide_modules(this->module_manager_.get_modules(), this->assigned_modules, &this->assigned_modules_count);  

        // mpi_communicator::scatter_ints(this->assigned_modules_count.data(), &this->assigned_count);

        // this->get_used_processes_count();

        // this->module_manager_.get_instructions(used_count);

        // this->distribute_modules();
    }

    void get_used_processes_count() {
        this->used_count = this->process_count > this->module_manager_.get_modules_count() ? 
                            this->module_manager_.get_modules_count() : 
                            this->process_count;
    }

    /*
    * @brief sends assigned modules to their assigned process
    */
    void distribute_modules() {
        //send to each used process
        for (int i = 0; i < this->used_count; i++) {
        //send each module assigned
        std::vector<module*> nodes_modules = this->assigned_modules[i];
            for (int j = 0; j < nodes_modules.size(); j++) {
                module* mod = nodes_modules.at(j);

                if (i == this->my_rank) {
                    this->mpi_manager_.add_new_module(mod->get_name(), mod->get_pla(), this->my_rank, mod->get_var_count(), mod->get_function_column());
                    this->my_instructions = this->module_manager_.get_instructions_for_process(i);
                    continue;
                }

                this->mpi_manager_.send_module_info(mod, this->module_manager_.get_instructions_for_process(i), i);
            }
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

/*
* @brief represents the rest of processes that are not main.
*
* Their only job is to complete assigned instructions with assigned modules.
*/
class slave_process : public process {
public:
    slave_process(int rank) : process(rank) {}
    /*
    * @brief recieves modules with instructions from main_process
    */
    void process_information() override {
        // mpi_communicator::scatter_ints(nullptr, &this->assigned_count);

        // if (this->assigned_count > 0) {
        //     this->mpi_manager_.recieve_my_modules(this->assigned_count, this->my_rank, this->my_instructions);
        // }
    }
};
