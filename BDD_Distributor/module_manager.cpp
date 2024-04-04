#include "module_manager.h"
#include <cstdint>
#include <functional>
#include <string>


void module_manager::load(std::string confPath) {
    try {
        this->load_modules(confPath);
        this->load_pla();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

void module_manager::load_modules(std::string confPath)
{
    auto constexpr is_space = [](auto const character)
        {
            return static_cast<bool>(std::isspace(character));
        };

    auto file = std::ifstream(confPath);

    if (not file.is_open())
    {
        throw std::runtime_error("Error opening conf file");
        return;
    }

    // Read paths to modules
    std::string name, path, column, line;

    while (std::getline(file, line))
    {
        if (line[0] == '#' || line.empty()) {
            continue;
        }

        int space_index = line.find(" ");
        int pla_index = line.find(".pla");

        if (pla_index == std::string::npos) {
            break;
        }

        name = line.substr(0, space_index);
        path = line.substr(space_index + 1, pla_index + 3 - name.length());
        column = line.substr(pla_index + 5);

        module* mod = new module(name);
        mod->set_path(path);
        mod->set_function_column(std::stoi(column));

        this->module_mapping.emplace(mod->get_name(), this->modules.size());
        this->modules.push_back(mod);
    }

    // Read mapping
    do
    {
        auto const first
            = std::find_if_not(std::begin(line), std::end(line), is_space);
        auto const last = std::end(line);
        if (first == last || *first == '#')
        {
            // Skip empty line.
            continue;
        }

        auto const keyLast = std::find_if(first, last, is_space);
        auto const valFirst = keyLast == last
            ? last
            : std::find_if_not(keyLast + 1, last, is_space);
        auto key = std::string(first, keyLast);
        if (valFirst != last)
        {
            auto valLast = last;
            while (is_space(*(valLast - 1)))
            {
                --valLast;
            }
            auto val = std::string(valFirst, valLast);

            int digits = 0;
            int position;
            std::stringstream moduleName;
            for (std::size_t i = 0; i < val.size(); ++i) {
                if (val[i] == 'M') {
                    moduleName.str("");
                    moduleName << 'M';
                    position = i - digits;

                    for (size_t j = i + 1; j < val.size(); j++)
                    {
                        if (val[j] == 'V' || val[j] == 'M')
                        {
                            i = j - 1;
                            break;
                        }
                        moduleName << val[j];
                        digits++;
                    }
                    this->modules.at(this->module_mapping.at(moduleName.str()))->set_position(position);
                    this->modules.at(this->module_mapping.at(key))->add_son(this->modules.at(this->module_mapping.at(moduleName.str())));
                }
            }
            this->modules.at(this->module_mapping.at(key))->set_var_count(val.size() - digits);
        }

    } while (std::getline(file, line));
}

module_manager::~module_manager()
{
    for (int i = 0; i < this->modules.size(); i++) {
        delete this->modules.at(i);
    }

    for (int i = 0; i < this->separate_instructions.size(); i++)
    {
        delete this->separate_instructions.at(i);
    }

    this->modules.clear();
    this->separate_instructions.clear();
}

void module_manager::print_modules() {
    if (this->modules.empty()) { return; }

    for (module* mod : this->modules) {
        std::cout << mod->get_name() << " " << mod->get_path() << "\n";
        std::cout << "My position: " << mod->get_position() << "\n";
        mod->print_sons();
        std::cout << "\n";
    }
}

void module_manager::get_instructions(int process_count) {
    this->separate_instructions.resize(process_count > this->modules.size() ?
                                        this->modules.size() : process_count);

    std::sort(this->modules.begin(), this->modules.end(), [](module* a, module* b) { return a->get_priority() < b->get_priority(); });

    for (int i = 0; i < this->modules.size(); i++)
    {
        module* mod = this->modules.at(i);
        module* parent = mod->get_parent();
        if (!this->separate_instructions.at(mod->get_process_rank()))
        {
            this->separate_instructions.at(mod->get_process_rank()) = new std::stringstream;
        }

        // EXEC - module name - position of the module in parent
        *this->separate_instructions.at(mod->get_process_rank()) << "EXEC " << mod->get_name() << " " << mod->get_position() << "\n";
        if (parent)
        {
            if (!this->separate_instructions.at(parent->get_process_rank()))
            {
                this->separate_instructions.at(parent->get_process_rank()) = new std::stringstream;
            }

            if (mod->get_process_rank() == parent->get_process_rank()) { 
                // LINK - name of parent module - name of son module
                *this->separate_instructions.at(mod->get_process_rank()) << "LINK " << parent->get_name() << " " << mod->get_name() << "\n";               
            } else {
                // SEND - name of module - rank of the process to send
                *this->separate_instructions.at(mod->get_process_rank()) << "SEND " << mod->get_name() << " " << parent->get_process_rank() << "\n";
            
                // RECV - parent module name - rank of the process recieved from
                *this->separate_instructions.at(parent->get_process_rank()) << "RECV " << parent->get_name() << " " << mod->get_process_rank() << "\n";
            }
        }
        else
        {
            // END - module which gives answer
            *this->separate_instructions.at(mod->get_process_rank()) << "END " << mod->get_name() << "\n";
        }
    }
}

std::string module_manager::get_instructions_for_process(int process_rank) {
    if (process_rank >= 0 && process_rank < this->separate_instructions.size()) {
        return this->separate_instructions.at(process_rank)->str();
    }
    return "INVALID RANK";
}


std::vector<module*>* module_manager::get_modules_for_process(int process_rank) {
    std::vector<module*>* nodes_modules = new std::vector<module*>();
    for (module* mod : this->modules) {
        if (mod->get_process_rank() == process_rank) {
            nodes_modules->push_back(mod);
        }
    }
    return nodes_modules;
}

void module_manager::print_assigned_processes()
{
    if (this->modules.empty()) { return; }

    for (module* mod : this->modules)
    {
        std::cout << mod->get_name() << " " << mod->get_process_rank() << "\n";
    }
}

void module_manager::print_separate_instructions()
{
    for (int i = 0; i < this->separate_instructions.size(); i++)
    {
        std::cout << "Node " << i << " instructions:\n";
        std::cout << this->separate_instructions.at(i)->str() << "\n";
    }
}

void module_manager::print_module_pla()
{
    for (module* mod : this->modules)
    {
        std::cout << mod->get_name() << std::endl;
        mod->print_pla();
        std::cout << std::endl;
    }
}

void module_manager::load_pla()
{
    if (this->modules.empty()) { return; }

    for (module* mod : this->modules)
    {
        auto file = std::ifstream(mod->get_path());

        if (not file.is_open())
        {   
            throw std::runtime_error("Error opening " + mod->get_name() + " PLA file");
            return;
        }

        std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        mod->set_pla(file_content);
        
        file.close();

    }
}


