#include "mpi_manager.h"
#include <ostream>

mpi_manager::~mpi_manager() {
    for (auto& pair : this->my_modules) { 
        delete pair.second; 
    }

    my_modules.clear();
}

void mpi_manager::evaluate(std::string module_name) {
    module* mod = this->my_modules.at(module_name);
    if (mod) {
        std::cout << "Density of " << this->calculated_state << ": " << mod->get_reliability() << std::endl;
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
        teddy::bss_manager::diagram_t f = bss_manager.from_pla(*pla_file, teddy::fold_type::Left)[mod->get_function_column()];
        //const double reliability = bss_manager.state_frequency(f, this->calculated_state);

        const double reliability = this->calculated_state == 1 ? 
                                    bss_manager.calculate_availability(1, *mod->get_sons_reliability(), f) :
                                    bss_manager.calculate_unavailability(1, *mod->get_sons_reliability(), f);
        std::cout << mod->get_name() << std::endl;
        for (int i = 0; i < mod->get_sons_reliability()->size(); i++) {
            for (int j = 0; j < mod->get_sons_reliability()->at(i).size(); j++) {
                std::cout << mod->get_sons_reliability()->at(i).at(j) << " ";
            }
            std::cout << std::endl;
        }

        // std::cout << "avail 1 " << bss_manager.calculate_availability(1, *mod->get_sons_reliability(), f) << std::endl;
        // std::cout << "avail 0 " << bss_manager.calculate_availability(0, *mod->get_sons_reliability(), f) << std::endl;
        // std::cout << "unavail 1 " << bss_manager.calculate_unavailability(1, *mod->get_sons_reliability(), f) << std::endl;
        // std::cout << "unavail 0 " << bss_manager.calculate_unavailability(0, *mod->get_sons_reliability(), f) << std::endl;
        // std::cout << "state 1 " << bss_manager.state_frequency(f, 1) << std::endl;
        // std::cout << "state 0 " << bss_manager.state_frequency(f, 0) << std::endl;

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
        mpi_communicator::send_int(mod->get_position(), recievers_rank);
        mpi_communicator::send_double(mod->get_reliability(), recievers_rank);
    } else {
        std::cout << "No module found\n";
    }
}

void mpi_manager::recv_module(std::string parent_name, int sender) {
    module* parent = this->my_modules.at(parent_name);
    if (parent) {
        int son_position = mpi_communicator::recv_int(sender);
        double son_rel = mpi_communicator::recv_double(sender);
        
        parent->set_sons_reliability(son_position, son_rel, this->calculated_state);
    } else {
        std::cout << "No module found\n";
    }
}

void mpi_manager::complete_instructions(std::string instructions, int state) {

    this->calculated_state = state;

    auto input_string = std::istringstream(instructions);

    std::string key_word, param_first, param_second;

    while (input_string >> key_word) {
        if (key_word == "END") {
            this->evaluate(param_first);
        } else {
            input_string >> param_first >> param_second;
            if (key_word == "EXEC") {
                this->execute_module(param_first, std::stoi(param_second));
            } else if (key_word == "LINK") {
                this->link_modules(param_first, param_second);
            } else if (key_word == "SEND") {
                this->send_module(param_first, std::stoi(param_second));
            } else if (key_word == "RECV") {
                this->recv_module(param_first, std::stoi(param_second));
            }
        }
    }
}

void mpi_manager::recieve_my_modules(int pa_my_assigned_modules, int pa_my_rank, std::string& pa_my_instructions) {
    for (int i = 0; i < pa_my_assigned_modules; i++) {

        std::string module_name = mpi_communicator::recv_string(0);
        std::string module_pla = mpi_communicator::recv_string(0);
        //pa_my_instructions = mpi_communicator::recv_string(0);
        //std::cout << pa_my_instructions << std::endl;
        int var_count = mpi_communicator::recv_int(0);
        int function_column = mpi_communicator::recv_int(0);

        this->add_new_module(module_name, module_pla, pa_my_rank, var_count, function_column);
                    
    }
    pa_my_instructions = mpi_communicator::recv_string(0);
    std::cout << pa_my_instructions << std::endl;
} 

void mpi_manager::send_module_info(module* mod, /*std::string instructions,*/ int recvRank) {
    mpi_communicator::send_string(mod->get_name(), recvRank);
    mpi_communicator::send_string(mod->get_pla(), recvRank);
    //mpi_communicator::send_string(instructions, recvRank);
    mpi_communicator::send_int(mod->get_var_count(), recvRank);
    mpi_communicator::send_int(mod->get_function_column(), recvRank);
}

void mpi_manager::add_new_module(std::string name, std::string pla, int my_rank, int var_count, int function_column) {
    module* temp = new module(name, 2);
    temp->set_var_count(var_count);
    temp->set_function_column(function_column);
    temp->set_pla(pla);
    temp->set_path(this->pla_path + "PROCESS " + std::to_string(my_rank) + "/" + name + ".pla");
    this->my_modules.emplace(name, temp);
}

void mpi_manager::write_to_pla() {
    for (auto& pair : this->my_modules) { 
        pair.second->write_pla_file(); 
    }
}

void mpi_manager::print_my_modules(int my_rank) {
    std::cout << my_rank << std::endl;
    for (auto pair : this->my_modules) {
        std::cout << pair.second->get_name() << std::endl;
    }
}
