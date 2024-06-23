#include "pla_function.h"
#include <ostream>
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
        std::cout << "Loop: " << i << std::endl;
        for (int j = 0; j < this->fun_values->size(); j++) {
            if (this->fun_values->at(j) == i) {
                new_function->push_back(this->variables->at(j));
                std::cout << "Line: ";
                std::cout << this->variables->at(j) << std::endl;
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
            std::cout << this->variables->at(j) << std::endl;
            if (this->variables->at(j)[position] == '-' && i == 0) {
                whatever_lines->push_back(this->variables->at(j));
            }
            if (this->variables->at(j)[position] - '0' == i) {
                new_function->push_back(this->variables->at(j));
            }
        }
        sorted->push_back(new_function);
    }
    sorted->push_back(whatever_lines);
    return sorted;
}

void pla_function::input_variables(std::vector<std::vector<std::string>*>* additional_vars, int position, int function) {
    std::vector<std::string>* new_vars = new std::vector<std::string>();
    std::vector<int>* new_fun_vals = new std::vector<int>();

    std::vector<std::vector<std::string>*>* my_vars = this->sort_by_position(position);
    std::string temp_line, input_line;
    int var_count = additional_vars->at(0)->at(0).length();
    int fun_value;
    std::string whatever_input = std::string(var_count, '-');

    for (int i = 0; i < my_vars->size(); i++) {
        std::cout << "Position: " << i << std::endl;
        std::cout << "Number of loops: " << my_vars->at(i)->size() << std::endl;
        if (i == my_vars->size() - 1) {
            for (int l = 0; l < my_vars->at(i)->size(); l++) {
                
                temp_line = my_vars->at(i)->at(l);
                fun_value = this->get_fun_value(temp_line);


                temp_line = this->replace_char(temp_line, position, whatever_input);

                new_vars->push_back(temp_line);
                new_fun_vals->push_back(fun_value);
                std::cout << temp_line << " " << fun_value << std::endl;
            }
            continue;
        }
        for (int j = 0; j < my_vars->at(i)->size(); j++) {
            temp_line = my_vars->at(i)->at(j);
            fun_value = this->get_fun_value(temp_line);
            std::cout << "Additional:" << additional_vars->at(i)->size() << std::endl;
            for (int k = 0; k < additional_vars->at(i)->size(); k++) {
                temp_line = my_vars->at(i)->at(j);
                input_line = additional_vars->at(i)->at(k);
                std::cout << input_line << std::endl;

                temp_line = this->replace_char(temp_line, position, input_line);

                new_vars->push_back(temp_line);
                new_fun_vals->push_back(fun_value);
                std::cout << temp_line << " " << fun_value << std::endl;
            }
        }
    }
    
    this->variables = new_vars;
    this->fun_values = new_fun_vals;

}

int pla_function::get_fun_value(std::string indexed_variables) {
     // Find the iterator to the element
    auto it = std::find(this->variables->begin(), this->variables->end(), indexed_variables);

    int value = 0;
    
    // Check if the element was found
    if (it != this->variables->end()) {
        // Calculate the index
        int index = std::distance(this->variables->begin(), it);
        value = this->fun_values->at(index);
    }

    return value;
}

std::string pla_function::replace_char(std::string before, int position, std::string input) {
    std::string before_part = before.substr(0, position);
    std::string after_part = before.substr(position + 1);
    return before_part + input + after_part;
}