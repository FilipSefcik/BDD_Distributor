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