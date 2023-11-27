#pragma once
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
	int nodeNum;
public:
	Node(PlaManager PaPlaManager, int paNodeNum);
	~Node();
	void loadPla();
	void writePla();
	int getTrueDensity();
};

