#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

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
	double my_reliability = 0.5;
public:
	module(std::string pa_name);
	~module(){ delete this->sons_reliability; };

	// getters

	module* get_parent() { return this->parent; };

	std::string get_name() { return this->name; };
	std::string get_path() { return this->path; };
    std::string get_pla() { return this->pla_file; };

	double get_reliability() { return this->my_reliability; };
	std::vector<std::vector<double>>* get_sons_reliability() { return this->sons_reliability; };

	int get_priority() { return this->priority; };
	int get_process_rank() { return this->assigned_process; };
	int get_position() { return this->position; };
    int get_var_count() { return this->var_count; };

	// setters

	void set_parent(module* pa_parent) { this->parent = pa_parent; };

	void set_path(std::string pa_path) { this->path = pa_path; };
	void set_pla(std::string pla_content) { this->pla_file = pla_content; };

    void set_reliability(double pa_rel) {this->my_reliability = pa_rel; };
    void set_sons_reliability(int son_position, double son_rel, int state);

	void set_position(int pa_position) { this->position = pa_position; };
	void assign_process(int pa_process) { this->assigned_process = pa_process; };
	void set_var_count(int pa_var_count);

	void add_son(module* new_son);
	void add_priority(int son_priority);


	void print_pla();
	void print_sons();
    void write_pla_file();
	void print_sons_reliabilities();
};

