#include <iostream>
#include <libteddy/core.hpp>
#include "PlaManager.h"

int main() {
	PlaManager pla("PLA-Files/load/simplePLA.pla");

	std::cout << pla.getLineCount() << std::endl;
	pla.printLines();
	std::cout << std::endl;

	//pla.writeToFiles("PLA-Files/write/", 2);

	std::vector<PlaManager> smallManagers = pla.divideToInstances(2);

	for (PlaManager manager : smallManagers)
	{
		std::cout << manager.getLineCount() << std::endl;
		manager.printLines();
		std::cout << std::endl;
	}


	return 0;
}