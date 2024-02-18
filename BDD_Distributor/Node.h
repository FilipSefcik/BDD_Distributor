#pragma once
#include <string>
#include "PlaManager.h"
#include "libteddy/core.hpp"
#include "libteddy/reliability.hpp"

/**
*@brief Simulates function of a node would in MPI communication
*/
class Node
{
	using diagram = teddy::bss_manager::diagram_t;
private:
	PlaManager* plaManager;
	teddy::bss_manager* bssManager;
	diagram function;
	int nodeIP;

public:
	Node(PlaManager PaPlaManager, int paNodeIP);
	~Node();
	void loadPla();
	void writePla();
	double getTrueDensity();
	int getIP() { return this->nodeIP; }
};

