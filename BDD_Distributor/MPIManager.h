#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>
#include <mpi.h>
#include <iostream>
#include "Module.h"
#include <libteddy/details/diagram_manager.hpp>
#include <libteddy/reliability.hpp>

class MPIManager {
private:
    std::unordered_map<std::string, Module*> my_modules;
    std::string PLA_PATH = "MOD_PLA/";
    int calculated_state = 1;

public:
    ~MPIManager();
    void evaluate(std::string module_name);
    void executeModule(std::string module_name, int module_position);
    void linkModules(std::string parent_name, std::string son_name);
    void sendModule(std::string module_name, int recievers_rank);
    void recvModule(std::string parent_name, int sender);
    void recieveMyModules(int pa_my_assigned_modules, int pa_my_rank, std::string& pa_my_instructions);
    void complete_instruction(std::string instructions, int value);
    void addNewModule(std::string name, std::string pla, int my_rank, int var_count);
    void sendString(std::string message, int recvRank);
    void sendInt(int message, int recvRank);
    void sendModuleInfo(Module* mod, std::string instructions, int recvRank);
    std::string recvString(int sendRank);
    int recvInt(int sendRank);
    void writeToPLA();
    void printMyModules(int my_rank);
};