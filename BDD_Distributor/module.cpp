#include "module.h"

module::module(std::string pa_name) {
    this->name = pa_name;
    this->sons_reliability = new std::vector<std::vector<double>>();
}

void module::add_son(module* new_son)
{
    new_son->set_parent(this);
    this->add_priority(new_son->get_priority());
}

void module::print_sons()
{
    std::cout << "Var count: " << this->var_count << std::endl;
    std::cout << "Priority: " << this->priority << std::endl;

    for (int i = 0; i < this->sons_reliability->size(); i++)
    {
        for (int j = 0; j < this->sons_reliability->at(i).size(); j++)
        {
            std::cout << this->sons_reliability->at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }

}

void module::add_priority(int son_priority)
{
    if (this->priority == son_priority)
    {
        this->priority++;
        if (this->parent) 
        {
            this->parent->add_priority(this->priority);
        }
    }
}

void module::print_pla() {
    std::cout << this->pla_file << std::endl;
}

void module::set_var_count(int pa_var_count) {
    this->var_count = pa_var_count;
    for (int i = 0; i < this->var_count; i++)
    {
        this->sons_reliability->push_back({0.5, 0.5});
    }
}

void module::set_sons_reliability(int sonPosition, double sonRel, int state) {
    if (sonPosition >= 0 && sonPosition < this->sons_reliability->size()) {
        this->sons_reliability->at(sonPosition).at(state) = sonRel;
        this->sons_reliability->at(sonPosition).at((state + 1) % 2) = 1.0 - sonRel;
    }
}

void module::write_pla_file() {

    std::filesystem::path dir_path = std::filesystem::path(this->path).parent_path();

    if (!std::filesystem::exists(dir_path)) {
        if (!std::filesystem::create_directories(dir_path)) {
            std::cerr << "Error creating directories!" << std::endl;
            exit(4);
            return; // Return an error code
        }
    }

    std::ofstream output_file(this->path);

    if (!output_file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        exit(4);
        return; 
    }

    output_file << this->pla_file << std::endl;

    output_file.close();
}

void module::print_sons_reliabilities() {
    for (int i = 0; i < this->sons_reliability->size(); i++) {
        for (int j = 0; j < this->sons_reliability->at(i).size(); j++) {
            std::cout << this->sons_reliability->at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
}