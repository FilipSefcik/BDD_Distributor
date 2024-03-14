#include <iostream>
#include "ModuleManager.h"
#include "Divider.h"
#include "MPIManager.h"
#include <mpi.h>
#include <vector>


int main(int argc, char* argv[]) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int process_count;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    // Get the rank of the process
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // rank 0, na ktorom sa pracuje, musí najprv
    // spracovať config súbor modulov

    std::vector<Node*> processes, used_processes;
    
    int my_assigned_modules_count;

    std::vector<int> assignedCount;
    assignedCount.resize(process_count);

    std::string module_name, module_pla, my_instructions;

    MPIManager mpiManager;

    if (my_rank == 0)
    {
        for (int i = 0; i < process_count; i++) {
            processes.push_back(new Node(i));
        }

        ModuleManager moduleManager;
	    moduleManager.loadModules("/home/sefcik1/BP_projects/BDD_Distributor/BDD_Distributor/Modules/module_map.conf");
        moduleManager.loadPLA();

        NodeDivider divider;
	    divider.divideModules(moduleManager.getModules(), &processes);    


        for (int i = 0; i < process_count; i++) {
            assignedCount.at(i) = processes.at(i)->getModulesCount();
            if (assignedCount.at(i) == 0) {
                delete processes.at(i);
            } else {
                used_processes.push_back(processes.at(i));
            }
        } 

        MPI_Scatter(assignedCount.data(), 1, MPI_INT, &my_assigned_modules_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        moduleManager.getInstructions(used_processes.size());

        //send to each used process
        for (int i = 0; i < used_processes.size(); i++) {
            //send each module assigned
            for (int j = 0; j < used_processes.at(i)->getModulesCount(); j++) {
                Module* mod = used_processes.at(i)->getModule(j);

                if (i == my_rank) {
                    mpiManager.addNewModule(mod->getName(), mod->getPLA(), my_rank, mod->getVarCount());
                    my_instructions = moduleManager.getInstructionFor(i);
                    continue;
                }

                mpiManager.sendModuleInfo(mod, moduleManager.getInstructionFor(i), used_processes.at(i)->getRank());
            }
        }

        for (int i = 0; i < used_processes.size(); i++) {
            delete used_processes.at(i);
        }
        processes.clear();
        used_processes.clear();

    } else {
        MPI_Scatter(assignedCount.data(), 1, MPI_INT, &my_assigned_modules_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (my_assigned_modules_count > 0) {
            for (int i = 0; i < my_assigned_modules_count; i++) {

                module_name = mpiManager.recvString(0);
                module_pla = mpiManager.recvString(0);
                my_instructions = mpiManager.recvString(0);
                int var_count = mpiManager.recvInt(0);

                mpiManager.addNewModule(module_name, module_pla, my_rank, var_count);
                    
            }
        }
    }

    if (my_assigned_modules_count > 0) { 
        mpiManager.writeToPLA();
        mpiManager.complete_instruction(my_instructions);
    }

    //Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}
