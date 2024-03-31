#pragma once
#include "process.h"
#include <mpi.h>

class bdd_distributor 
{
private:
    int my_rank, process_count;
    process* process_;

public:
    bdd_distributor();
    void set_conf_path(std::string pa_conf_path);
    void calculate_availability(int divider_flag, int state);
    ~bdd_distributor();
};