#include "MPIManager.h"
#include <cstdint>
#include <libteddy/details/diagram_manager.hpp>
#include <libteddy/reliability.hpp>
#include <string>

void MPIManager::evaluate(std::string module_name) {
    Module* mod = this->my_modules.at(module_name);
    if (mod) {
        std::cout << "True density: " << mod->getReliability() << std::endl;
    } else {
        std::cout << "Module not found.\n";
    }
}

void MPIManager::executeModule(std::string module_name, int module_position) {
    Module* mod = this->my_modules.at(module_name);
    if (mod) {
        mod->setPosition(module_position);

        std::string const& cesta = mod->getPath();
        teddy::bss_manager bssManager(mod->getVarCount(), 1'000);
        std::cout << cesta << std::endl;
        std::optional<teddy::pla_file> plaSubor = teddy::pla_file::load_file(cesta);
        std::cout << "loaded pla " << &plaSubor << std::endl;

        // spustit viac procesov mozete aj pomocou:   // mpiexec -n 2 BDD_Distributor/main //
        // ak ste v adresari build/, kde 2 znamená počet prcesov

        // ERROR !!!!!!!!!
        teddy::bss_manager::diagram_t f = bssManager.from_pla(*plaSubor, teddy::fold_type::Left)[0];
        // ERROR !!!!!!!!!

        std::cout << "made a function\n";
        const double reliability = bssManager.calculate_availability(1, *mod->getSonsReliability(), f);

        mod->setReliability(reliability);
        std::cout << "Module " << mod->getName() << " reliability is " << reliability << " with position " << mod->getPosition() << std::endl;
    } else {
        std::cout << "Module not found.\n";
    }
}

void MPIManager::linkModules(std::string parent_name, std::string son_name) {
    Module* parent = this->my_modules.at(parent_name);
    Module* son = this->my_modules.at(son_name);
    if (parent && son) {
        parent->setSonsReliability(son->getPosition(), son->getReliability());
        std::cout << "Linked modules " << parent->getName() << " <-> " << son->getName() << std::endl;
    } else {
        std::cout << "No module found.\n";
    }
}

void MPIManager::sendModule(std::string module_name, int recievers_rank) {
    Module* mod = this->my_modules.at(module_name);
    if (mod) {
        int mod_position = mod->getPosition();
        double mod_rel = mod->getReliability();

        MPI_Send(&mod_position, 1, MPI_INT, recievers_rank, 0, MPI_COMM_WORLD);
        MPI_Send(&mod_rel, 1, MPI_DOUBLE, recievers_rank, 0, MPI_COMM_WORLD);
        std::cout << "Sent module " << mod->getName() << " " << mod_position << " " << mod_rel << std::endl;
    } else {
        std::cout << "No module found\n";
    }
}

void MPIManager::recvModule(std::string parent_name, int sender) {
    Module* parent = this->my_modules.at(parent_name);
    if (parent) {
        int son_position;
        double son_rel;
        //int var_count;

        MPI_Recv(&son_position, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&son_rel, 1, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //MPI_Recv(&var_count, 1, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        parent->setSonsReliability(son_position, son_rel);

        std::cout << "Recieved module on position " << son_position << " " << son_rel << std::endl;
    } else {
        std::cout << "No module found\n";
    }
}


void MPIManager::complete_instruction(std::string instructions) {
    
    auto input_string = std::istringstream(instructions);

    std::string keyWord, paramFirst, paramSecond;

    while (input_string >> keyWord) {
        if (keyWord == "END") {
            input_string >> paramFirst;
            this->evaluate(paramFirst);
        } else {
            input_string >> paramFirst >> paramSecond;
            if (keyWord == "EXEC") {
                this->executeModule(paramFirst, std::stoi(paramSecond));
            } else if (keyWord == "LINK") {
                this->linkModules(paramFirst, paramSecond);
            } else if (keyWord == "SEND") {
                this->sendModule(paramFirst, std::stoi(paramSecond));
            } else if (keyWord == "RECV") {
                this->recvModule(paramFirst, std::stoi(paramSecond));
            }
        }
    }
}

void MPIManager::sendString(std::string message, int recvRank) {
    int size = message.size() + 1;
    MPI_Send(&size, 1, MPI_INT, recvRank, 0, MPI_COMM_WORLD);
    MPI_Send(message.c_str(), size, MPI_CHAR, recvRank, 0, MPI_COMM_WORLD);
}

void MPIManager::sendInt(int message, int recvRank) {
    MPI_Send(&message, 1, MPI_INT, recvRank, 0, MPI_COMM_WORLD);
}

int MPIManager::recvInt(int sendRank) {
    int message;
    MPI_Recv(&message, 1, MPI_INT, sendRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}

void MPIManager::sendModuleInfo(Module* mod, std::string instructions, int recvRank) {
    sendString(mod->getName(), recvRank);
    sendString(mod->getPLA(), recvRank);
    sendString(instructions, recvRank);
    sendInt(mod->getVarCount(), recvRank);
}

std::string MPIManager::recvString(int sendRank) {
    int size;
    char* message;
    MPI_Recv(&size, 1, MPI_INT, sendRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    message = (char*)malloc(sizeof(char)*size);
    MPI_Recv(message, size, MPI_CHAR, sendRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}

void MPIManager::addNewModule(std::string name, std::string pla, int my_rank, int var_count) {
    Module* temp = new Module(name);
    temp->setVarCount(var_count);
    temp->setPLA(pla);
    temp->setPath(this->PLA_PATH + "PROCESS " + std::to_string(my_rank) + "/" + name + ".pla");
    this->my_modules.emplace(name, temp);
    //std::cout << temp->getName() << " " << temp->getVarCount() << std::endl;
}

void MPIManager::writeToPLA() {
    for (auto& pair : this->my_modules) { 
        pair.second->writePLAFile(); 
    }
}

MPIManager::~MPIManager() {
    for (auto& pair : this->my_modules) { 
        delete pair.second; 
    }

    my_modules.clear();
}

void MPIManager::printMyModules(int my_rank) {
    std::cout << my_rank << std::endl;
    for (auto pair : this->my_modules) {
        std::cout << pair.second->getName() << std::endl;
    }
}
