#pragma once
#include <string>
#include "PlaManager.h"
#include "libteddy/core.hpp"

/**
*@brief Simulates function of a node would in MPI communication
*/
class Node
{
	using diagram = teddy::bdd_manager::diagram_t;
private:
	PlaManager* plaManager;
	teddy::bdd_manager* bddManager;
	diagram function;
	int nodeIP;

public:
	Node(PlaManager PaPlaManager, int paNodeIP);
	~Node();
	void loadPla();
	void writePla();
	int getTrueDensity();
	int getIP() { return this->nodeIP; }
};

