#pragma once
#include "mpi_communicator.h"
#include "process.h"
#include <cstddef>
#include <limits>

/*
 * @brief bdd_distributor is used as an interface to control processing of config file
 *
 * You can set a path to config file,
 * start calculating availability where you define used divider, value that you want to be
 * calculated and usage of timer, recieve the time of how long the calculating took.
 */
class bdd_distributor {
  private:
    int my_rank, process_count;
    double start_time, end_time;
    process* process_;

  public:
    bdd_distributor();
    void set_conf_path(std::string pa_conf_path);
    void calculate_availability(int divider_flag, int state, bool timer_on);
    void get_max_time();
    ~bdd_distributor();
};