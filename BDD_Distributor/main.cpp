#include <iostream>
#include "ModuleManager.h"
#include "Divider.h"

#include <mpi.h>

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

    std::vector<Node*> processes;

    int my_assigned_modules_count;
    std::vector<int> assignedCount;
    assignedCount.resize(process_count);

    // ked sa posiela string tak ku velkosti pridat +1 vzdy!!!

    if (my_rank == 0)
    {
        /* kod na spracovanie a rozdelenie modulov */
        printf("Som proces %d a prave spravovavam moduly\n", my_rank);
        printf("Som proces %d a viem kolko je modulov, rozdelil ich a vytvoril instrukcie\n", my_rank);

        for (int i = 0; i < process_count; i++) {
            processes.push_back(new Node(i));
        }

        ModuleManager moduleManager;
	    moduleManager.loadModules("Modules/module_map.conf");
        moduleManager.loadPLA();

        // moduleManager.printModules();
        // moduleManager.prinModulePLA();

        NodeDivider divider;
	    divider.divideModules(moduleManager.getModules(), &processes);

        // for (int i = 0; i < processes.size(); i++)
        // {
	    //     processes.at(i)->printModules();
        // }      

        for (int i = 0; i < process_count; i++) {
            assignedCount.at(i) = processes.at(i)->getModulesCount();
            //printf("%d\n",processes.at(i)->getModulesCount());
        } 

        moduleManager.getInstructions(process_count);

        //moduleManager.printSeparateInstructions();

        printf("Som proces %d a idem poslat kolko modulov ma spracovat kazdy proces\n", my_rank);
        //MPI_Scatter (assignedCount.data(),1,MPI_INT,my_assigned_modules,1,MPI_INT,0,MPI_COMM_WORLD)

    } else {
        printf("Som proces %d a idem prijat kolko modulov mam spracovat\n", my_rank);
    }

    //scatter oznami aj samemu sebe, takze modul 0 bude mat v premennej my_assigned_modules hodnotu z assignedCount(0)
    MPI_Scatter(assignedCount.data(), 1, MPI_INT, &my_assigned_modules_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Som proces %d a mam spracovat %d modulov\n", my_rank, my_assigned_modules_count);

    if (my_assigned_modules_count != 0)
    {

        if (my_rank == 0)
        {
            printf("Som proces %d a idem poslat moduly s prisluchajucim PLA\n", my_rank);

            /*
                std::string message;
                int recv_rank;
                for each (moduly) {
                    message = module.getName();
                    recv_rank = module.getNodeRank();

                    // najprv musime informovat ake dlhe su tie stringy
                    MPI_Send(message.size(), 1, MPI_INT, recv_rank, 0, MPI_COMM_WORLD);
                    // poslanie stringu
                    MPI_Send(message.cstr(), message.size(), MPI_CHAR, recv_rank, 0, MPI_COMM_WORLD);

                    message = module.getPLA();
                    MPI_Send(message.size(), 1, MPI_INT, recv_rank, 0, MPI_COMM_WORLD);
                    // poslanie stringu
                    MPI_Send(message.cstr(), message.size(), MPI_CHAR, recv_rank, 0, MPI_COMM_WORLD);

                }           
            
            */
            

        } else {
            /*
                int string_size; 
                char* message;
                char* PLA;
                for (int i; i < my_assigned_modules; i++) {
                    //prijanie velkosti stringu
                    MPI_Recv(string_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    //prijat string
                    MPI_Recv(message, string_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                    //prijanie velkosti stringu PLA
                    MPI_Recv(string_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    //prijat string
                    MPI_Recv(PLA, string_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                }
            */

        }
        
    } else {
        printf("Som proces %d a nemam pridelene ziadne ulohy :(\n", my_rank);
    }


    // Finalize the MPI environment.
    MPI_Finalize();
}
