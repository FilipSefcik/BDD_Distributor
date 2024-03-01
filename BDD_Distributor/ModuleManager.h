#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <vector>
#include "Divider.h"
#include "Module.h"
class ModuleManager
{
private:
	std::unordered_map<std::string, Module*> modules;
	std::vector<Module*> vector_modules;
	std::vector<std::stringstream*> separate_instructions;
public:
	~ModuleManager();
	
	std::unordered_map<std::string, Module*>* getModules() { return &this->modules; };
	void getInstructions(std::vector<Node*>* nodes);

	void loadModules(std::string confPath);
	void loadPLA();

	void printModules();
	void prinModulePLA();
	void printAssignedNodes();
	void printSeparateInstructions();
};

