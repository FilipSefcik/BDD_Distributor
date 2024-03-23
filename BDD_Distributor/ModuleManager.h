#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <unordered_map>
#include <vector>
#include "Module.h"

class ModuleManager
{
private:
	std::vector<Module*> modules;
	std::vector<std::stringstream*> separate_instructions;
	std::unordered_map<std::string, int> module_mapping;
public:
	~ModuleManager();
	
	std::vector<Module*>* getModules() { return &this->modules; };
	void getInstructions(int nodesCount);
    std::string getInstructionFor(int node_rank);
	std::vector<Module*>* getModulesForNode(int node_rank);

	void loadModules(std::string confPath);
	void loadPLA();

	void printModules();
	void printModulePLA();
	void printAssignedNodes();
	void printSeparateInstructions();
};

