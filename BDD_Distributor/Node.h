#pragma once
#include "PlaManager.h"
#include "libteddy/core.hpp"

class Node
{
private:
	PlaManager* plaManager;
	teddy::bdd_manager* bddManager;
public:
	Node(PlaManager PaPlaManager);
	~Node();
};

