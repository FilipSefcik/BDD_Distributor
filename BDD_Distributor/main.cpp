#include <cstdint>
#include <iostream>
#include "Module.h"
#include "ModuleManager.h"
#include "Divider.h"
#include "MPIManager.h"
#include <mpi.h>
#include <vector>

// temporary funtions, will be refactored, used for easier reading
int getUsedProcessesCount(std::vector<int>* pa_assigned_modules) {
    int count = 0;
    for (int i = 0; i < pa_assigned_modules->size(); i++) {
        if (pa_assigned_modules->at(i) > 0) {
            count++;
        } else {
            // may be changed depending on what divider will be used
            break;
        }
    }
    return count;
}

void distributeModules(int used_processes_count, int pa_my_rank, std::string& pa_my_instructions, ModuleManager* pa_mod_manager, MPIManager* pa_mpi_manager) {
    //send to each used process
    for (int i = 0; i < used_processes_count; i++) {
        //send each module assigned
        std::vector<Module*>* nodes_modules = pa_mod_manager->getModulesForNode(i);
        for (int j = 0; j < nodes_modules->size(); j++) {
            Module* mod = nodes_modules->at(j);

            if (i == pa_my_rank) {
                pa_mpi_manager->addNewModule(mod->getName(), mod->getPLA(), pa_my_rank, mod->getVarCount());
                pa_my_instructions = pa_mod_manager->getInstructionFor(i);
                continue;
            }

            pa_mpi_manager->sendModuleInfo(mod, pa_mod_manager->getInstructionFor(i), i);
        }
        delete nodes_modules;
    }
}

// without MPI
/*
int main(int argc, char* argv[]) {

    int process_count = 1;
    int my_rank = 0;
    
    int my_assigned_modules_count;

    std::string my_instructions;

    MPIManager mpiManager;

    std::vector<int> assigned_modules;
    assigned_modules.resize(process_count);
    int used_processes_count;

    ModuleManager moduleManager;
	moduleManager.loadModules("/home/sefcik1/BP_projects/BDD_Distributor/BDD_Distributor/Modules/module_map.conf");
    moduleManager.loadPLA();

    // moduleManager.printModules();
    // moduleManager.printModulePLA();

    NodeDivider divider;
	divider.divideModules(moduleManager.getModules(), &assigned_modules);  

    // moduleManager.printAssignedNodes();
        
    my_assigned_modules_count = assigned_modules.at(0);
        
    used_processes_count = getUsedProcessesCount(&assigned_modules);

    moduleManager.getInstructions(used_processes_count);
        
    moduleManager.printSeparateInstructions();  

    distributeModules(used_processes_count, my_rank, my_instructions, &moduleManager, &mpiManager);

    if (my_assigned_modules_count > 0) { 
        mpiManager.writeToPLA();
        mpiManager.complete_instruction(my_instructions);
    }

    return 0;
}*/


//with MPI
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
    
    int my_assigned_modules_count;

    std::string my_instructions;

    MPIManager mpiManager;

    if (my_rank == 0)
    {
        std::vector<int> assigned_modules;
        assigned_modules.resize(process_count);
        int used_processes_count;

        ModuleManager moduleManager;
	    moduleManager.loadModules("/home/sefcik1/BP_projects/BDD_Distributor/BDD_Distributor/Modules/module_map.conf");
        moduleManager.loadPLA();

        // moduleManager.printModules();
        // moduleManager.printModulePLA();

        NodeDivider divider;
	    divider.divideModules(moduleManager.getModules(), &assigned_modules);  

        // moduleManager.printAssignedNodes();

        MPI_Scatter(assigned_modules.data(), 1, MPI_INT, &my_assigned_modules_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        used_processes_count = getUsedProcessesCount(&assigned_modules);

        moduleManager.getInstructions(used_processes_count);
        
        // moduleManager.printSeparateInstructions();  

        distributeModules(used_processes_count, my_rank, my_instructions, &moduleManager, &mpiManager);

    } else {
        MPI_Scatter(nullptr, 1, MPI_INT, &my_assigned_modules_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (my_assigned_modules_count > 0) {
            mpiManager.recieveMyModules(my_assigned_modules_count, my_rank, my_instructions);
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
