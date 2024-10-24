#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include "pla_function.h"

/*
* @brief Class module describes a module in config file 
*
* It's a bniary function described by PLA file but 
* each variable can have different probability of 1 and 0.
*/
class module
{
private:
	std::string name, path, pla_file;
	std::vector<std::vector<double>>* sons_reliability;
	module* parent = nullptr;
	int assigned_process = 0;
	int position = 0;
	int priority = 0; // number of modules between the furthest "child" module and this module
	int var_count = 0;
	int function_column = 0;
	double my_reliability = 0.5;
	std::unordered_map<std::string, int> sons_map;
	pla_function* function;
public:
	module(std::string pa_name);
	~module(){ delete this->sons_reliability; delete this->function; };

	// getters

	module* get_parent() { return this->parent; };
	pla_function* get_pla_function() { return this->function; };

	std::string get_name() { return this->name; };
	std::string get_path() { return this->path; };
    std::string get_pla() { return this->pla_file; };

	double get_reliability() { return this->my_reliability; };
	std::vector<std::vector<double>>* get_sons_reliability() { return this->sons_reliability; };

	int get_priority() { return this->priority; };
	int get_process_rank() { return this->assigned_process; };
	int get_position() { return this->position; };
    int get_var_count() { return this->var_count; };
	int get_function_column() { return this->function_column; };

	// setters

	void set_parent(module* pa_parent) { this->parent = pa_parent; };

	void set_path(std::string pa_path) { this->path = pa_path; };
	void set_pla(std::string pla_content) { this->pla_file = pla_content; };

    void set_reliability(double pa_rel) {this->my_reliability = pa_rel; };
    void set_sons_reliability(int son_position, double son_rel, int state);

	void set_position(int pa_position) { this->position = pa_position; };
	void assign_process(int pa_process) { this->assigned_process = pa_process; };
	void set_var_count(int pa_var_count);
	void set_function_column(int pa_column) { this->function_column = pa_column; };

	// special functions

	void add_son(module* new_son);
	void add_priority(int son_priority);

	// prints used to get info
	// used only during troubleshooting


	int get_son_position(std::string son_name) { return this->sons_map.at(son_name); };
	void add_son_position(std::string son_name, int position) { this->sons_map.emplace(son_name, position); };
	void edit_position(std::string son_name, int new_position) { this->sons_map.at(son_name) = new_position; };


	void print_pla();
	void print_sons();
    void write_pla_file();
	void print_sons_reliabilities();
};

