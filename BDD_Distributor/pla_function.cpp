#include "pla_function.h"
#include <vector>

std::vector<std::string>* pla_function::get_vars_from_fun(int function_value) {
    std::vector<std::string>* all_vars = new std::vector<std::string>;

    for (int i = 0; i < this->fun_values.size(); i++) {
        if (this->fun_values.at(i) == function_value) {
            all_vars->push_back(this->variables.at(i));
        }
    }
    
    return all_vars;
}

void pla_function::add_line(std::string new_vars, int value)  {
    this->add_variables(new_vars); 
    this->add_fun_value(value); 
}

void pla_function::print_function() {
    std::cout << "variables       function\n";
    for (int i = 0; i < this->fun_values.size(); i++) {
        std::cout << this->variables.at(i) << " " << this->fun_values.at(i) << std::endl;
    }
}