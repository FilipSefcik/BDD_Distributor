#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

class Module
{
private:
	std::string name;
	std::string path;
	std::string pla_file;
	std::vector<std::vector<double>>* sons_reliability;
	Module* parent = nullptr;
	int assignedNode;
	int position = 0;
	int priority = 0; // number of modules between the furthest "child" module and this module
	int varCount = 0;
	double my_reliability = 0.0;
public:
	Module(std::string paName);
	~Module(){ delete this->sons_reliability; };

	std::string getName() { return this->name; };
	std::string getPath() { return this->path; };
	std::string getPLA() { return this->pla_file; };
	double getReliability() { return this->my_reliability; };
	std::vector<std::vector<double>>* getSonsReliability() { return this->sons_reliability; };
	int getPriority() { return this->priority; };
	Module* getParent() { return this->parent; };
	int getNodeRank() { return this->assignedNode; };
	int getPosition() { return this->position; };

	void setPath(std::string paPath) { this->path = paPath; };
	void setPosition(int paPosition) { this->position = paPosition; };
	void setParent(Module* paParent) { this->parent = paParent; };
	void assignNode(int paNode) { this->assignedNode = paNode; };
	void setPLA(std::string plaFileContent) { this->pla_file = plaFileContent; };

	void addSon(int position, Module* newSon);
	void setVarCount(int paVarCount);
	void addPriority(int sonPriority);


	void printPLA();
	void printSons();
};

