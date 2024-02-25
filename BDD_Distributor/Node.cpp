#include "Node.h"

Node::Node(PlaManager PaPlaManager, int paNodeNum)
{
	this->nodeIP = paNodeNum;
	this->plaManager = new PlaManager(PaPlaManager);
	this->bssManager = new teddy::bss_manager(this->plaManager->getVarCount(), this->plaManager->getVarCount() * 1000);
	this->assignedModules = new std::vector<Module*>();
	//this->loadPla();
}

Node::~Node()
{
	delete this->plaManager;
	//delete this->bddManager; //throws error
	delete this->assignedModules;
}

void Node::loadPla()
{
	auto plaFile = teddy::pla_file::load_file("PLA-Files/node" + std::to_string(this->nodeIP) + "/file0.pla");
	this->function = this->bssManager->from_pla(*plaFile, teddy::fold_type::Left)[0];
}

void Node::writePla()
{
	this->plaManager->writeToFiles("PLA-Files/node" + std::to_string(this->nodeIP) + "/", 1);
}

double Node::getTrueDensity()
{
	std::vector<std::array<double, 2>> ps ({
		{0.5, 0.5}, // A 50% - 0, 50% - 1 
		{0.5, 0.5}, // B 50% - 0, 50% - 1
		{0.25, 0.75} // C 25% - 0, 75% - 1
		});

	return this->bssManager->calculate_availability(1, ps, this->function);
}

void Node::printModules()
{
	std::cout << "Node " << this->nodeIP << std::endl;
	std::cout << "Assigned modules:";
	for (Module* module : *this->assignedModules)
	{
		std::cout << " " << module->getName();
	}
	std::cout << std::endl;
}


