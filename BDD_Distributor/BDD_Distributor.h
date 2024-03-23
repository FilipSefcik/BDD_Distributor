#pragma once
#include <cstdint>
#include <iostream>
#include "Module.h"
#include "ModuleManager.h"
#include "Divider.h"
#include "MPIManager.h"
#include <mpi.h>
#include <vector>

class BDD_Distributor 
{
private:
    int my_rank, process_count, my_assigned_modules_count;
    std::string conf_path;
    MPIManager mpiManager;

public:
    BDD_Distributor();
    void set_conf_path(std::string pa_conf_path) { this->conf_path = pa_conf_path; };
    void calculate_availability(int divider_flag, int state);
    ~BDD_Distributor();
};