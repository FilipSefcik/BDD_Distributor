#pragma once
#include "Module.h"
class ModuleManager
{
private:
	std::hash_map<std::string, Module> modules;
public:
	void loadModules(std::string confPath);
};

