#include <iostream>
#include <libteddy/core.hpp>
#include "PlaManager.h"
#include "Distributor.h"

int main() {
	PlaManager pla("PLA-Files/load/simplePLA.pla");

	//std::cout << "Function: F=(A+B)*C\n";
	std::cout << "Start: \n";
	std::cout << pla.getLineCount() << std::endl;
	pla.printLines();
	std::cout << std::endl;

	pla.writeToFiles("PLA-Files/write/", 2);

	Distributor distributor;

	std::vector<PlaManager> smallManagers = pla.divideToInstances(distributor.getNodeCount());

	std::cout << "Small managers: \n";
	for (PlaManager manager : smallManagers)
	{
		std::cout << manager.getLineCount() << std::endl;
		//manager.printLines();
		std::cout << std::endl;
	}

	distributor.loadNodes(smallManagers);

	std::cout << "True density: \n";
	std::cout << distributor.evaluateNodes() << std::endl;


	return 0;
}