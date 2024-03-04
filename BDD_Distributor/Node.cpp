#include "Node.h"

Node::Node(int paNodeNum)
{
	this->node_rank = paNodeNum;
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
	std::ofstream file(this->node_rank + "process/" + path);
	if (!file.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
		return; // Return an error code
	}
	file << content;
	file.close();
}


double Node::getTrueDensity(std::string moduleName)
{
	Module* module = this->findModule(moduleName);

	if (!module)
	{
		std::cout << "Can't find module.";
		return 0.0;
	}

	return this->bssManager->calculate_availability(1, *module->getSonsReliability(), this->function);
}

void Node::printModules()
{
	std::cout << "Node " << this->node_rank << std::endl;
	std::cout << "Assigned modules:";
	for (Module* module : *this->assignedModules)
	{
		std::cout << " " << module->getName();
	}
	std::cout << std::endl;
}

Module* Node::findModule(std::string moduleName)
{
	if (!this->assignedModules || this->assignedModules->empty())
	{
		return nullptr;
	}

	for (int i = 0; i < this->assignedModules->size(); i++)
	{
		if (this->assignedModules->at(i)->getName() == moduleName)
		{
			return this->assignedModules->at(i);
		}
	}

	return nullptr;
}

Module* Node::getModule(int index)
{
	return index >= 0 && 
		   index < this->assignedModules->size() ?
		   this->assignedModules->at(index) :
		   nullptr;
}


