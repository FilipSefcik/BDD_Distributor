#include "Distributor.h"

Distributor::Distributor()
{
    std::srand(144);
    this->nodeCount = /*(std::rand() % 5) + 1*/ 5;
}

Distributor::~Distributor()
{
	for (int i = 0; i < this->nodes.size(); i++)
	{
		delete this->nodes.at(i);
	}
}

void Distributor::loadNodes(std::vector<PlaManager> smallManagers)
{
	for (int i = 0; i < smallManagers.size(); i++)
	{
		smallManagers.at(i).writeToFiles("PLA-Files/node" + std::to_string(i) + "/", 1);
		this->nodes.push_back(new Node(smallManagers.at(i), i));
	}
}

double Distributor::evaluateNodes()
{
	double trueDensity = 0;

	for (Node* node : this->nodes)
	{
		trueDensity += node->getTrueDensity();
	}

	return trueDensity;
}

int Distributor::getNodeCount()
{
	return this->nodeCount;
}
