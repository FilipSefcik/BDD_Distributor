#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Module.h"
#include <libteddy/reliability.hpp>

/**
*@brief Simulates function of a node would in MPI communication
*/
class Node {
private:
	//teddy::bss_manager* bssManager;
	std::vector<Module*>* assignedModules;
	//teddy::bss_manager::diagram_t function;
	int node_rank;

public:
	Node(int paNodeNum);
	~Node();
	int getRank() { return this->node_rank; }
	int getModulesCount() { return this->assignedModules->size(); };
	double getTrueDensity(std::string moduleName);
    Module* getModule(int index);


	void loadPla(std::string path);
	void writePla(std::string path, std::string content);
	void printModules();
	void assignModule(Module* paModule) { this->assignedModules->push_back(paModule); };
	Module* findModule(std::string moduleName);
    std::string getString(int index);
};

