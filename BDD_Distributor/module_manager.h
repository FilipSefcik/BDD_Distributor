#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <unordered_map>
#include <vector>
#include "module.h"

class module_manager
{
private:
	std::vector<module*> modules;
	std::vector<std::stringstream*> separate_instructions;
	std::unordered_map<std::string, int> module_mapping;
public:
	~module_manager();
	
	std::vector<module*>* get_modules() { return &this->modules; };
	void get_instructions(int process_count);
    std::string get_instructions_for_process(int process_rank);
	std::vector<module*>* get_modules_for_process(int process_rank);

	void load(std::string conf_path);
	void load_modules(std::string conf_path);
	void load_pla();

	void print_modules();
	void print_module_pla();
	void print_assigned_processes();
	void print_separate_instructions();
};
