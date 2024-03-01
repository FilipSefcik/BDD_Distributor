#include <iostream>
#include <libteddy/core.hpp>
//#include "PlaManager.h"
//#include "Distributor.h" // not used since 18.2. 14:35
#include "ModuleManager.h"
#include "Divider.h"

// added line to test main branch

int main() {

	std::vector<Node*> nodes;

	for (int i = 0; i < 6; i++)
	{
		nodes.push_back(new Node(i));
	}

	ModuleManager moduleManager;
	moduleManager.loadModules("Modules/module_map.conf");
	std::cout << "Loaded modules: \n";
	moduleManager.printModules();
	std::cout << "====================\n";

	std::cout << "Loaded PLA: \n";
	moduleManager.loadPLA();
	moduleManager.prinModulePLA();

	std::cout << "====================\n";

	NodeDivider divider;
	divider.divideModules(moduleManager.getModules(), &nodes);

	std::cout << "Divided modules: \n";

	for (int i = 0; i < nodes.size(); i++)
	{
		nodes.at(i)->printModules();
	}

	std::vector<int> assignedCount;
	assignedCount.resize(nodes.size());

	std::cout << "Divided modules count: \n";

	for (int i = 0; i < nodes.size(); i++)
	{
		assignedCount.at(i) = nodes.at(i)->getModulesCount();
		std::cout << i << " " << assignedCount.at(i) << std::endl;
	}

	std::cout << "====================\n";
	std::cout << "Assigned Nodes: \n";
	moduleManager.printAssignedNodes();
	std::cout << "====================\n";
	moduleManager.getInstructions(&nodes);
	std::cout << "====================\n";
	std::cout << "Separate instructions: \n";
	moduleManager.printSeparateInstructions();

	/*for (int i = 0; i < nodeMaps->size(); i++)
	{
		std::cout << nodeMaps->at(i)->module->getName() << " => " << nodeMaps->at(i)->node->getIP() << std::endl;
	}*/


	//deleting dynamic pointers

	for (int i = 0; i < nodes.size(); i++)
	{
		delete nodes.at(i);
	}

	return 0;
}