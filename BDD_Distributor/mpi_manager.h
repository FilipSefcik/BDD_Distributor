#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>
#include <mpi.h>
#include <iostream>
#include "module.h"
#include "mpi_communicator.h"
#include <libteddy/details/diagram_manager.hpp>
#include <libteddy/reliability.hpp>

class mpi_manager {
private:
    mpi_communicator communicator;
    std::unordered_map<std::string, module*> my_modules;
    std::string pla_path = "MOD_PLA/";
    int calculated_state = 1;

public:
    ~mpi_manager();

    void evaluate(std::string module_name);
    void execute_module(std::string module_name, int module_position);
    void link_modules(std::string parent_name, std::string son_name);
    void send_module(std::string module_name, int recievers_rank);
    void recv_module(std::string parent_name, int sender);

    void recieve_my_modules(int pa_my_assigned_modules, int pa_my_rank, std::string& pa_my_instructions);
    void complete_instructions(std::string instructions, int state);
    void add_new_module(std::string name, std::string pla, int my_rank, int var_count, int function_column);

    void send_module_info(module* mod, std::string instructions, int recievers_rank);

    void write_to_pla();
    void print_my_modules(int my_rank);
};