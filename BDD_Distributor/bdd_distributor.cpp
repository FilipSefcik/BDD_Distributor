#include "bdd_distributor.h"

bdd_distributor::bdd_distributor() {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &this->process_count);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &this->my_rank);

    if (this->my_rank == 0) {
        this->process_ = new main_process(this->my_rank, this->process_count);
    } else {
        this->process_ = new slave_process(this->my_rank);
    }
}

bdd_distributor::~bdd_distributor() {
    delete this->process_;
    //Finalize the MPI environment.
    MPI_Finalize();
}

void bdd_distributor::set_conf_path(std::string pa_conf_path) {
    if (this->my_rank == 0 && this->process_) {
        main_process* mainProcess = dynamic_cast<main_process*>(this->process_);
        mainProcess->set_conf_path(pa_conf_path);
    }
};

void bdd_distributor::calculate_availability(int divider_flag, int state) {
    if (state != 0 && state != 1) {
        std::cerr << "Invalid state" << std::endl;
        exit(2);
        return;
    }

    if (this->my_rank == 0 && this->process_) {
        main_process* mainProcess = dynamic_cast<main_process*>(this->process_);
        mainProcess->set_divider(divider_flag);
    }
    this->process_->process_information();
    this->process_->process_instructions(state);
};
