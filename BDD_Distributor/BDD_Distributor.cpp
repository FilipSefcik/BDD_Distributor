#include "BDD_Distributor.h"
#include "Process.h"

BDD_Distributor::BDD_Distributor() {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &this->process_count);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &this->my_rank);

    if (this->my_rank == 0) {
        this->process = new MainProcess(this->my_rank, this->process_count);
    } else {
        this->process = new SlaveProcess(this->my_rank);
    }
}

BDD_Distributor::~BDD_Distributor() {
    delete this->process;
    //Finalize the MPI environment.
    MPI_Finalize();
}

void BDD_Distributor::set_conf_path(std::string pa_conf_path) {
    if (this->my_rank == 0 && this->process) {
        MainProcess* mainProcess = dynamic_cast<MainProcess*>(this->process);
        mainProcess->setConfPath(pa_conf_path);
    }
};

void BDD_Distributor::calculate_availability(int divider_flag, int state) {
    if (this->my_rank == 0 && this->process) {
        MainProcess* mainProcess = dynamic_cast<MainProcess*>(this->process);
        mainProcess->setDivider(divider_flag);
    }
    this->process->process_information();
    this->process->process_instructions(state);
};
