#pragma once
#include "Node.h"

/**
* @brief Manages whole proces of loading pla files and distributing them using MPI (not yet)
*/
class Distributor
{
private:
	std::vector<Node*> nodes;
	int nodeCount;
public:
	Distributor();
	~Distributor();
	void loadNodes(std::vector<PlaManager> smallManagers);
	double evaluateNodes();
	int getNodeCount();
};

