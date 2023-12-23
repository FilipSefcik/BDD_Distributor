#pragma once
#include <string>
#include <unordered_map>
class Module
{
private:
	std::string name;
	std::string path;
	std::unordered_map<int, Module*> sons;
public:
	Module(std::string paName, std::string paPath)
		: name(paName), path(paPath) {};
	~Module();
	std::string gatName() { return this->name; };
	std::string getPath() { return this->path; };
};

