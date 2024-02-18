#include <iostream>
#include <libteddy/core.hpp>
#include "PlaManager.h"
#include "Distributor.h" // not used since 18.2. 14:35
#include "ModuleManager.h"
#include "Divider.h"

int main() {
	PlaManager pla("PLA-Files/load/simplePLA.pla");

	std::vector<Node*> nodes;

	for (int i = 0; i < 3; i++)
	{
		nodes.push_back(new Node(pla, i));
	}

	ModuleManager moduleManager;
	moduleManager.loadModules("Modules/module_map.conf");
	std::cout << "Loaded modules: \n";
	moduleManager.printModules();
	std::cout << "====================\n";

	std::vector<NodeMap*>* nodeMaps = new std::vector<NodeMap*>();

	NodeDivider divider;
	divider.divideModules(moduleManager.getModules(), &nodes, nodeMaps);

	std::cout << "Divided modules: \n";

	for (int i = 0; i < nodeMaps->size(); i++)
	{
		std::cout << nodeMaps->at(i)->module->gatName() << " => " << nodeMaps->at(i)->node->getIP() << std::endl;
	}


	//deleting dynamic pointers

	for (int i = 0; i < nodeMaps->size(); i++)
	{
		delete nodeMaps->at(i);
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		delete nodes.at(i);
	}

	return 0;
}