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
	Module* parent = nullptr;
	int priority = 0; // number of modules between the furthest "child" module and this module
	int varCount = 0;
	double reliability = 0.0;
public:
	Module(std::string paName, std::string paPath) :
		name(paName), path(paPath) {};
	~Module();
	std::string getName() { return this->name; };
	std::string getPath() { return this->path; };
	double getReliability() { return this->reliability; };
	int getPriority() { return this->priority; };
	void setParent(Module* paParent) { this->parent = paParent; };
	void addSon(int position, Module* newSon);
	void printSons();
	void setVarCount(int paVarCount) { this->varCount = paVarCount; }
	void addPriority(int sonPriority);
};

