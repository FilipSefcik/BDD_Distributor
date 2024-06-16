#include "pla_function.h"
#include <string>
#include <vector>

pla_function::pla_function() {
    this->variables = new std::vector<std::string>();
    this->fun_values = new std::vector<int>();
}

pla_function::~pla_function() {
    delete this->variables;
    delete this->fun_values;
}

std::vector<std::string>* pla_function::get_vars_from_fun(int function_value) {
    std::vector<std::string>* all_vars = new std::vector<std::string>;

    for (int i = 0; i < this->fun_values->size(); i++) {
        if (this->fun_values->at(i) == function_value) {
            all_vars->push_back(this->variables->at(i));
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
    for (int i = 0; i < this->fun_values->size(); i++) {
        std::cout << this->variables->at(i) << " " << this->fun_values->at(i) << std::endl;
    }
}

std::vector<std::vector<std::string>*>* pla_function::sort_by_function() {
    std::vector<std::vector<std::string>*>* sorted = new std::vector<std::vector<std::string>*>();
    for (int i = 0; i < 2; i++) {
        std::vector<std::string>* new_function = new std::vector<std::string>();
        for (int j = 0; j < this->fun_values->size(); j++) {
            if (this->fun_values->at(j) == i) {
                new_function->push_back(this->variables->at(j));
            }
        }
        sorted->push_back(new_function);
    }
    return sorted;
}

std::vector<std::vector<std::string>*>* pla_function::sort_by_position(int position) {
    std::vector<std::vector<std::string>*>* sorted = new std::vector<std::vector<std::string>*>();
    std::vector<std::string>* whatever_lines = new std::vector<std::string>();
    for (int i = 0; i < 2; i++) {
        std::vector<std::string>* new_function = new std::vector<std::string>();
        for (int j = 0; j < this->variables->size(); j++) {
            if (this->variables->at(j)[position] - '0' == i) {
                new_function->push_back(this->variables->at(j));
            }
            if (this->variables->at(j)[position] == '-') {
                whatever_lines->push_back(this->variables->at(j));
            }
        }
        sorted->push_back(new_function);
    }
    sorted->push_back(whatever_lines);
    return sorted;
}

void pla_function::input_variables(std::vector<std::string> *additional_vars, int position, int function) {
    std::vector<std::string>* new_vars = new std::vector<std::string>();
    std::vector<int>* new_fun_vals = new std::vector<int>();

    for (int i = 0; this->variables->size(); i++) {
        std::string line = this->variables->at(i);
        if (line[position] - '0' != function) { continue; }
        for (int j = 0; additional_vars->size(); j++) {

        }
    }
}