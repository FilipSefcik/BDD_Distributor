#include "MPIManager.h"
#include <mpi.h>

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
        //std::cout << cesta << std::endl;
        std::optional<teddy::pla_file> plaSubor = teddy::pla_file::load_file(cesta);
        //std::cout << "loaded pla " << cesta << std::endl;

        //mod->printSonsReliabilities();

        teddy::bss_manager::diagram_t f = bssManager.from_pla(*plaSubor, teddy::fold_type::Left)[0];

        //std::cout << "made a function\n";

        const double reliability = this->calculated_state == 1 ? 
                                    bssManager.calculate_availability(1, *mod->getSonsReliability(), f) :
                                    bssManager.calculate_unavailability(1, *mod->getSonsReliability(), f);

        //std::cout << this->calculated_state << std::endl;
        mod->setReliability(reliability);
        //std::cout << "Module " << mod->getName() << " reliability is " << reliability << " with position " << mod->getPosition() << std::endl;
    } else {
        std::cout << "Module not found.\n";
    }
}

void MPIManager::linkModules(std::string parent_name, std::string son_name) {
    Module* parent = this->my_modules.at(parent_name);
    Module* son = this->my_modules.at(son_name);
    if (parent && son) {
        parent->setSonsReliability(son->getPosition(), son->getReliability(), this->calculated_state);
        //std::cout << "Linked modules " << parent->getName() << " <-> " << son->getName() << std::endl;
    } else {
        std::cout << "No module found.\n";
    }
}

void MPIManager::sendModule(std::string module_name, int recievers_rank) {
    Module* mod = this->my_modules.at(module_name);
    if (mod) {
        //std::cout << "Sending to " << recievers_rank << std::endl;

        this->communicator.sendInt(mod->getPosition(), recievers_rank);
        this->communicator.sendDouble(mod->getReliability(), recievers_rank);

        //std::cout << "Sent module " << mod->getName() << " " << mod_position << " " << mod_rel << std::endl;
    } else {
        std::cout << "No module found\n";
    }
}

void MPIManager::recvModule(std::string parent_name, int sender) {
    Module* parent = this->my_modules.at(parent_name);
    if (parent) {
        //int son_position;
        //double son_rel;

        //std::cout << "Recieving from " << sender << std::endl;
        int son_position = this->communicator.recvInt(sender);
        double son_rel = this->communicator.recvDouble(sender);
        
        parent->setSonsReliability(son_position, son_rel, this->calculated_state);

        //std::cout << "Recieved module on position " << son_position << " " << son_rel << std::endl;
    } else {
        std::cout << "No module found\n";
    }
}


void MPIManager::complete_instruction(std::string instructions, int state) {
    if (state != 0 && state != 1) {
        std::cout << "Invalid state\n";
        return;
    }

    this->calculated_state = state;
    
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

void MPIManager::recieveMyModules(int pa_my_assigned_modules, int pa_my_rank, std::string& pa_my_instructions) {
    std::string module_name, module_pla;
    for (int i = 0; i < pa_my_assigned_modules; i++) {

        module_name = this->communicator.recvString(0);
        module_pla = this->communicator.recvString(0);
        pa_my_instructions = this->communicator.recvString(0);
        int var_count = this->communicator.recvInt(0);

        this->addNewModule(module_name, module_pla, pa_my_rank, var_count);
                    
     }
} 

void MPIManager::sendModuleInfo(Module* mod, std::string instructions, int recvRank) {
    this->communicator.sendString(mod->getName(), recvRank);
    this->communicator.sendString(mod->getPLA(), recvRank);
    this->communicator.sendString(instructions, recvRank);
    this->communicator.sendInt(mod->getVarCount(), recvRank);
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
