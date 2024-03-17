#include <cstdint>
#include <iostream>
#include "Module.h"
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

    //std::cout << &process_count << std::endl;

    // Get the rank of the process
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // rank 0, na ktorom sa pracuje, musí najprv
    // spracovať config súbor modulov

    //std::vector<Node*> processes, used_processes;
    
    int my_assigned_modules_count;

    std::string module_name, module_pla, my_instructions;

    MPIManager mpiManager;

    if (my_rank == 0)
    {
        std::vector<int> assigned_modules;
        assigned_modules.resize(process_count);
        int used_processes_count;

        ModuleManager moduleManager;
	    moduleManager.loadModules("/home/sefcik1/BP_projects/BDD_Distributor/BDD_Distributor/Modules/module_map.conf");
        moduleManager.loadPLA();

        moduleManager.printModules();
        moduleManager.printModulePLA();


        NodeDivider divider;
	    divider.divideModules(moduleManager.getModules(), &assigned_modules);  

        moduleManager.printAssignedNodes();

        MPI_Scatter(assigned_modules.data(), 1, MPI_INT, &my_assigned_modules_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = 0; i < process_count; i++) {
            if (assigned_modules.at(i) > 0) {
                used_processes_count++;
            } else {
                // may be changed depending on what divider method will be used
                break;
            }
        }

        moduleManager.getInstructions(used_processes_count);
        
        moduleManager.printSeparateInstructions();  
    
        //send to each used process
        for (int i = 0; i < used_processes_count; i++) {
            //send each module assigned
            std::vector<Module*>* nodes_modules = moduleManager.getModulesForNode(i);
            for (int j = 0; j < nodes_modules->size(); j++) {
                Module* mod = nodes_modules->at(j);

                if (i == my_rank) {
                    mpiManager.addNewModule(mod->getName(), mod->getPLA(), my_rank, mod->getVarCount());
                    my_instructions = moduleManager.getInstructionFor(i);
                    continue;
                }

                mpiManager.sendModuleInfo(mod, moduleManager.getInstructionFor(i), i);
            }
            delete nodes_modules;
        }

    } else {
        MPI_Scatter(nullptr, 1, MPI_INT, &my_assigned_modules_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

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
