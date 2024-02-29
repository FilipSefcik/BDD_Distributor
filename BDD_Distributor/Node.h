#pragma once
#include <string>
#include "Module.h"
#include "PlaManager.h"
#include <libteddy/core.hpp>
#include <libteddy/reliability.hpp>

/**
*@brief Simulates function of a node would in MPI communication
*/
class Node
{
	using diagram = teddy::bss_manager::diagram_t;
private:
	teddy::bss_manager* bssManager;
	std::vector<Module*>* assignedModules;
	diagram function;
	int nodeIP;

public:
	Node(int paNodeNum);
	~Node();
	void loadPla(std::string path);
	void writePla(std::string path, std::string content);
	double getTrueDensity();
	void printModules();
	int getIP() { return this->nodeIP; }
	int getModulesCount() { return this->assignedModules->size(); };
	void assignModule(Module* paModule) { this->assignedModules->push_back(paModule); };
};

