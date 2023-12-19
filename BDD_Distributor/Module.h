#pragma once
#include <string>
#include <hash_map>
class Module
{
private:
	std::string name;
	std::string path;
	std::hash_map<int, Module> sons;
public:
	std::string gatName() { return this->name; };
};

