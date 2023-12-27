#include "Node.h"

Node::Node(PlaManager PaPlaManager, int paNodeNum)
{
	this->nodeIP = paNodeNum;
	this->plaManager = new PlaManager(PaPlaManager);
	this->bddManager = new teddy::bdd_manager(this->plaManager->getVarCount(), this->plaManager->getVarCount() * 1000);
	//this->loadPla();
}

Node::~Node()
{
	delete this->plaManager;
	//delete this->bddManager; //throws error
}

void Node::loadPla()
{
	auto plaFile = teddy::pla_file::load_file("PLA-Files/node" + std::to_string(this->nodeIP) + "/file0.pla");
	this->function = this->bddManager->from_pla(*plaFile, teddy::fold_type::Left)[0];
}

void Node::writePla()
{
	this->plaManager->writeToFiles("PLA-Files/node" + std::to_string(this->nodeIP) + "/", 1);
}

int Node::getTrueDensity()
{
	return this->bddManager->satisfy_count(1, this->function);
}


