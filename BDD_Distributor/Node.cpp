#include "Node.h"

Node::Node(PlaManager PaPlaManager)
{
	this->plaManager = new PlaManager(PaPlaManager);
	this->bddManager = new teddy::bdd_manager(this->plaManager->getVarCount(), this->plaManager->getVarCount() * 100);
}

Node::~Node()
{
	delete this->plaManager;
	delete this->bddManager;
}
