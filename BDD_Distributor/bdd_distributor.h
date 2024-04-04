#pragma once
#include "process.h"
#include <mpi.h>

class bdd_distributor 
{
private:
    int my_rank, process_count;
    double start_time, end_time, max_time;
    process* process_;

public:
    bdd_distributor();
    void set_conf_path(std::string pa_conf_path);
    void calculate_availability(int divider_flag, int state, bool timer_on);
    void get_max_time();
    ~bdd_distributor();
};