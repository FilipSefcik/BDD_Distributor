#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include "Divider.h"
#include "Module.h"
class ModuleManager
{
private:
	std::unordered_map<std::string, Module*> modules;
public:
	void loadModules(std::string confPath);
	~ModuleManager();
	void printModules();
	std::unordered_map<std::string, Module*>* getModules() { return &this->modules; };
	std::vector<std::string>* getInstructions(std::vector<Node*>* nodes);
};

