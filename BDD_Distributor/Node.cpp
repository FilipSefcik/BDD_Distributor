#include "Node.h"

Node::Node(int paNodeNum)
{
	this->nodeIP = paNodeNum;
	this->assignedModules = new std::vector<Module*>();
}

Node::~Node()
{
	delete this->assignedModules;
}

void Node::loadPla(std::string path)
{
	auto plaFile = teddy::pla_file::load_file(path);
	this->function = this->bssManager->from_pla(*plaFile, teddy::fold_type::Left)[0];
}

void Node::writePla(std::string path, std::string content)
{
	std::ofstream file(this->nodeIP + "process/" + path);
	if (!file.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
		return; // Return an error code
	}
	file << content;
	file.close();
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


