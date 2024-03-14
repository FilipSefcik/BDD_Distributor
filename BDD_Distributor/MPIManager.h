#pragma once
#include <string>
#include <unordered_map>
#include "Module.h"

#include <libteddy/reliability.hpp>
#include <mpi.h>

class MPIManager {
private:
    std::unordered_map<std::string, Module*> my_modules;
    std::string PLA_PATH = "MOD_PLA/";

public:
    ~MPIManager();
    void evaluate(std::string module_name);
    void executeModule(std::string module_name, int module_position);
    void linkModules(std::string parent_name, std::string son_name);
    void sendModule(std::string module_name, int recievers_rank);
    void recvModule(std::string parent_name, int sender);
    void complete_instruction(std::string instructions);
    void addNewModule(std::string name, std::string pla, int my_rank, int var_count);
    void sendString(std::string message, int recvRank);
    void sendInt(int message, int recvRank);
    void sendModuleInfo(Module* mod, std::string instructions, int recvRank);
    std::string recvString(int sendRank);
    int recvInt(int sendRank);
    void writeToPLA();
};