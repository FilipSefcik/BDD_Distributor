#pragma once
#include <vector>
#include <iostream>
#include "ModuleManager.h"
#include "Divider.h"
#include "MPIManager.h"

class Process {
protected:
    MPIManager mpiManager;
    std::string my_instructions;
    int my_rank, assigned_count;
public:
    Process(int rank) : my_rank(rank) {};
    virtual void process_information() = 0;
    void process_instructions(int state) {
        this->mpiManager.writeToPLA();
        this->mpiManager.complete_instruction(this->my_instructions, state);
    };
};

class MainProcess : public Process {
private:
    std::vector<int> assigned_modules;  
    int used_count = 0; 
    int process_count = 0;
    ModuleManager moduleManager;
    Divider* divider;
    std::string confPath;

public:
    MainProcess(int rank, int pa_process_count) : Process(rank), process_count(pa_process_count) {};

    ~MainProcess() { if (divider) { delete divider; } };

    void setConfPath(std::string path) { this->confPath = path; };

    void process_information() override {
        this->assigned_modules.resize(this->process_count);

	    this->moduleManager.load(this->confPath);

	    this->divider->divideModules(this->moduleManager.getModules(), &this->assigned_modules);  

        MPI_Scatter(this->assigned_modules.data(), 1, MPI_INT, &this->assigned_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        this->getUsedProcessesCount();

        this->moduleManager.getInstructions(used_count);

        this->distributeModules();
    }

    void getUsedProcessesCount() {
        for (int i = 0; i < this->assigned_modules.size(); i++) {
            if (this->assigned_modules.at(i) > 0) {
                this->used_count++;
            } else {
                // may be changed depending on what divider will be used
                break;
            }
        }
    }

    void distributeModules() {
        //send to each used process
        for (int i = 0; i < this->used_count; i++) {
        //send each module assigned
        std::vector<Module*>* nodes_modules = this->moduleManager.getModulesForNode(i);
            for (int j = 0; j < nodes_modules->size(); j++) {
                Module* mod = nodes_modules->at(j);

                if (i == this->my_rank) {
                    this->mpiManager.addNewModule(mod->getName(), mod->getPLA(), this->my_rank, mod->getVarCount());
                    this->my_instructions = this->moduleManager.getInstructionFor(i);
                    continue;
                }

                this->mpiManager.sendModuleInfo(mod, this->moduleManager.getInstructionFor(i), i);
            }
            delete nodes_modules;
        }
    }

    void setDivider(int flag) {
        switch (flag) {
            case 0:
                divider = new VarCountDivider();
                break;
            default:
                divider = new NodeDivider();
                break;
        }
    }
};

class SlaveProcess : public Process {
public:
    SlaveProcess(int rank) : Process(rank) {}
    void process_information() override {
        MPI_Scatter(nullptr, 1, MPI_INT, &this->assigned_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (this->assigned_count > 0) {
            this->mpiManager.recieveMyModules(this->assigned_count, this->my_rank, this->my_instructions);
        }
    }
};
