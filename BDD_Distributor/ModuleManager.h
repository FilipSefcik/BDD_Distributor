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
	std::vector<std::stringstream*> separate_instructions;
public:
	void loadModules(std::string confPath);
	~ModuleManager();
	void printModules();
	std::unordered_map<std::string, Module*>* getModules() { return &this->modules; };
	void getInstructions(std::vector<Node*>* nodes);
	void printAssignedNodes();
	void printSeparateInstructions();
	bool comparator(Module* a, Module* b);
	void loadModulePLA();
	void prinModulePLA();
};

