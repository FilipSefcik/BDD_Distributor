#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

class Module
{
private:
	std::string name;
	std::string path;
	std::unordered_map<int, Module*> sons;
	int varCount = 0;
	double reliability = 0.0;
public:
	Module(std::string paName, std::string paPath)
		: name(paName), path(paPath) {};
	~Module();
	std::string gatName() { return this->name; };
	std::string getPath() { return this->path; };
	double getReliability() { return this->reliability; };
	void addSon(int position, Module* newSon);
	void printSons();
	void setVarCount(int paVarCount) { this->varCount = paVarCount; }
};

