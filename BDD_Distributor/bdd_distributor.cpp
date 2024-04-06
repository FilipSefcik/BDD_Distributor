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
}

void bdd_distributor::calculate_availability(int divider_flag, int state, bool timer_on) {
    if (state != 0 && state != 1) {
        std::cerr << "Invalid state" << std::endl;
        exit(2);
        return;
    }

    if (timer_on) { this->start_time = MPI_Wtime(); }

    if (this->my_rank == 0 && this->process_) {
        main_process* mainProcess = dynamic_cast<main_process*>(this->process_);
        mainProcess->set_divider(divider_flag);
    }
    this->process_->process_information();
    this->process_->process_instructions(state);

    if (timer_on) { this->end_time = MPI_Wtime(); }
}

void bdd_distributor::get_max_time() {
    double min_start, max_end;

    std::vector<double> start_times, end_times;
    start_times.resize(this->process_count);
    end_times.resize(this->process_count);

    MPI_Gather(&this->start_time, 1, MPI_DOUBLE, start_times.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(&this->end_time, 1, MPI_DOUBLE, end_times.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (this->my_rank == 0) {
        min_start = std::numeric_limits<double>::max();
        max_end = std::numeric_limits<double>::min();
        for (int i = 0; i < this->process_count; i++) {
            min_start = min_start > start_times.at(i) ? start_times.at(i) : min_start;
            max_end = max_end < end_times.at(i) ? end_times.at(i) : max_end;
        }

        std::cout << "Time: " << max_end - min_start << std::endl;
    } 
}
