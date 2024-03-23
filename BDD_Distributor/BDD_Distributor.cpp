#include "BDD_Distributor.h"

BDD_Distributor::BDD_Distributor() {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &this->process_count);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &this->my_rank);
}

BDD_Distributor::~BDD_Distributor() {
    //Finalize the MPI environment.
    MPI_Finalize();
}