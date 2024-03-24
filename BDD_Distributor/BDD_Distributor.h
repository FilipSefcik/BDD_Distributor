#pragma once
#include "Process.h"
#include <mpi.h>

class BDD_Distributor 
{
private:
    int my_rank, process_count;
    Process* process;

public:
    BDD_Distributor();
    void set_conf_path(std::string pa_conf_path);
    void calculate_availability(int divider_flag, int state);
    ~BDD_Distributor();
};