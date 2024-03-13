#include "MPIManager.h"

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
        //double reliability = 0.0;

        //teddy - moze to byt aj v triede modul napr.

        // ERROR !!!!!!!!!

        std::string const& cesta = mod->getPath();
        teddy::bss_manager bssManager(mod->getVarCount(), 1'000);
        auto plaSubor = teddy::pla_file::load_file(cesta);
        teddy::bss_manager::diagram_t f = bssManager.from_pla(*plaSubor, teddy::fold_type::Left)[0];
        const double reliability = bssManager.calculate_availability(1, *mod->getSonsReliability(), f);
        
        // ERROR !!!!!!!!!

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

        MPI_Recv(&son_position, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&son_rel, 1, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
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

    // std::cout << "prikaz: " << keyWord << std::endl;
    // std::cout << "param 1: " << paramFirst << std::endl;
    // if (!paramSecond.empty()) {
    //     std::cout << "param 2: " << paramSecond << std::endl;
    // }

    /*
    auto constexpr is_space = [](auto const character)
        {
            return static_cast<bool>(std::isspace(character));
        };

    auto input_string = std::istringstream(instructions);

    auto line = std::string();
    std::cout << "moje instrukcie:\n";
    while (std::getline(input_string, line)) {

        auto const first
            = std::find_if_not(std::begin(line), std::end(line), is_space);
        auto const last = std::end(line);
        if (first == last)
        {
            // Skip empty line.
            continue;
        }

        auto const key = std::find_if(first, last, is_space);
        auto const valFirst = key == last
            ? last
            : std::find_if_not(key + 1, last, is_space);
        auto const valSecond = valFirst == last
            ? last
            : std::find_if_not(valFirst + 1, last, is_space);
        
        std::string keyWord = std::string(first, key);
        std::string paramFirst, paramSecond;
        if (valFirst != last)
        {
            auto valLast = std::find_if(valFirst, last, is_space);
            paramFirst = std::string(valFirst, valLast);
        }

        if (valSecond != last) {
            auto valLast = std::find_if(valSecond, last, is_space);
            paramSecond = std::string(valSecond, valLast);
        }

        std::cout << "prikaz: " << keyWord << std::endl;
        std::cout << "param 1: " << paramFirst << std::endl;
        if (!paramSecond.empty()) {
            std::cout << "param 2: " << paramSecond << std::endl;
        }

 
    }*/


}

void MPIManager::sendString(std::string message, int recvRank) {
    int size = message.size() + 1;
    MPI_Send(&size, 1, MPI_INT, recvRank, 0, MPI_COMM_WORLD);
    MPI_Send(message.c_str(), size, MPI_CHAR, recvRank, 0, MPI_COMM_WORLD);
}

void MPIManager::sendModuleInfo(Module* mod, std::string instructions, int recvRank) {
    sendString(mod->getName(), recvRank);
    sendString(mod->getPLA(), recvRank);
    sendString(instructions, recvRank);
}

std::string MPIManager::recvString(int sendRank) {
    int size;
    char* message;
    MPI_Recv(&size, 1, MPI_INT, sendRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    message = (char*)malloc(sizeof(char)*size);
    MPI_Recv(message, size, MPI_CHAR, sendRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}

void MPIManager::addNewModule(std::string name, std::string pla, int my_rank) {
    Module* temp = new Module(name);
    temp->setPLA(pla);
    temp->setPath(this->PLA_PATH + "PROCESS " + std::to_string(my_rank) + "/" + name + ".pla");
    this->my_modules.emplace(name, temp);
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
