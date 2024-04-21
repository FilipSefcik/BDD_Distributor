#pragma once
#include <sstream>
#include <algorithm>
#include <regex>
#include <cstdint>
#include <functional>
#include "module.h"

/*
* @brief manages all modules loaded from PLA file
*/

class module_manager
{
private:
	std::vector<module*> modules;
	std::vector<std::stringstream*> separate_instructions;
	std::unordered_map<std::string, int> module_mapping;
public:
	~module_manager();

	// getters
	std::vector<module*>* get_modules() { return &this->modules; };
	int get_modules_count() { return this->modules.size(); };

	//special funtions
	
	void get_instructions(int process_count);
    std::string get_instructions_for_process(int process_rank);

	//loading of modules and PLA files

	void load(std::string conf_path);
	void load_modules(std::string conf_path);
	void load_pla();

	// info prints used only for troubleshooting

	void print_modules();
	void print_module_pla();
	void print_assigned_processes();
	void print_separate_instructions();
};

