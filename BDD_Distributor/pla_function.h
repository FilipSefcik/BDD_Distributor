#pragma once
#include <string>
#include <vector>

class pla_function
{
private:
    std::vector<std::string> variables;
    std::vector<int> fun_values;

public:
    std::vector<std::string>* get_vars_from_fun(int function_value);

    void add_fun_value(int value) { this->fun_values.push_back(value); };
    void add_variables(std::string new_vars) { this->variables.push_back(new_vars); };
};