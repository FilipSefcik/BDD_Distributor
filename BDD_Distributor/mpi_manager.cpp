#include "mpi_manager.h"

void mpi_manager::evaluate(std::string module_name) {
    module* mod = this->my_modules.at(module_name);
    if (mod) {
        std::cout << "True density: " << mod->get_reliability() << std::endl;
    } else {
        std::cout << "module not found.\n";
    }
}

void mpi_manager::execute_module(std::string module_name, int module_position) {
    module* mod = this->my_modules.at(module_name);
    if (mod) {
        mod->set_position(module_position);

        std::string const& path = mod->get_path();
        teddy::bss_manager bss_manager(mod->get_var_count(), mod->get_var_count() * 100);
        std::optional<teddy::pla_file> pla_file = teddy::pla_file::load_file(path);
        teddy::bss_manager::diagram_t f = bss_manager.from_pla(*pla_file, teddy::fold_type::Left)[0];

        const double reliability = this->calculated_state == 1 ? 
                                    bss_manager.calculate_availability(1, *mod->get_sons_reliability(), f) :
                                    bss_manager.calculate_unavailability(1, *mod->get_sons_reliability(), f);

        mod->set_reliability(reliability);
    } else {
        std::cout << "module not found.\n";
    }
}

void mpi_manager::link_modules(std::string parent_name, std::string son_name) {
    module* parent = this->my_modules.at(parent_name);
    module* son = this->my_modules.at(son_name);
    if (parent && son) {
        parent->set_sons_reliability(son->get_position(), son->get_reliability(), this->calculated_state);
    } else {
        std::cout << "No module found.\n";
    }
}

void mpi_manager::send_module(std::string module_name, int recievers_rank) {
    module* mod = this->my_modules.at(module_name);
    if (mod) {
        this->communicator.send_int(mod->get_position(), recievers_rank);
        this->communicator.send_double(mod->get_reliability(), recievers_rank);
    } else {
        std::cout << "No module found\n";
    }
}

void mpi_manager::recv_module(std::string parent_name, int sender) {
    module* parent = this->my_modules.at(parent_name);
    if (parent) {
        int son_position = this->communicator.recv_int(sender);
        double son_rel = this->communicator.recv_double(sender);
        
        parent->set_sons_reliability(son_position, son_rel, this->calculated_state);
    } else {
        std::cout << "No module found\n";
    }
}


void mpi_manager::complete_instruction(std::string instructions, int state) {
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
                this->execute_module(paramFirst, std::stoi(paramSecond));
            } else if (keyWord == "LINK") {
                this->link_modules(paramFirst, paramSecond);
            } else if (keyWord == "SEND") {
                this->send_module(paramFirst, std::stoi(paramSecond));
            } else if (keyWord == "RECV") {
                this->recv_module(paramFirst, std::stoi(paramSecond));
            }
        }
    }
}

void mpi_manager::recieve_my_modules(int pa_my_assigned_modules, int pa_my_rank, std::string& pa_my_instructions) {
    std::string module_name, module_pla;
    for (int i = 0; i < pa_my_assigned_modules; i++) {

        module_name = this->communicator.recv_string(0);
        module_pla = this->communicator.recv_string(0);
        pa_my_instructions = this->communicator.recv_string(0);
        int var_count = this->communicator.recv_int(0);

        this->add_new_module(module_name, module_pla, pa_my_rank, var_count);
                    
     }
} 

void mpi_manager::send_module_info(module* mod, std::string instructions, int recvRank) {
    this->communicator.send_string(mod->get_name(), recvRank);
    this->communicator.send_string(mod->get_pla(), recvRank);
    this->communicator.send_string(instructions, recvRank);
    this->communicator.send_int(mod->get_var_count(), recvRank);
}


void mpi_manager::add_new_module(std::string name, std::string pla, int my_rank, int var_count) {
    module* temp = new module(name);
    temp->set_var_count(var_count);
    temp->set_pla(pla);
    temp->set_path(this->pla_path + "PROCESS " + std::to_string(my_rank) + "/" + name + ".pla");
    this->my_modules.emplace(name, temp);
}

void mpi_manager::write_to_pla() {
    for (auto& pair : this->my_modules) { 
        pair.second->write_pla_file(); 
    }
}

mpi_manager::~mpi_manager() {
    for (auto& pair : this->my_modules) { 
        delete pair.second; 
    }

    my_modules.clear();
}

void mpi_manager::print_my_modules(int my_rank) {
    std::cout << my_rank << std::endl;
    for (auto pair : this->my_modules) {
        std::cout << pair.second->get_name() << std::endl;
    }
}
