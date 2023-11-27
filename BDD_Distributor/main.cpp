#include <iostream>
#include <libteddy/core.hpp>
#include "PlaManager.h"

int main() {
	PlaManager pla("PLA-Files/load/simplePLA.pla");

	std::cout << pla.getLineCount() << std::endl;
	pla.printLines();

	pla.writeToFiles("PLA-Files/write/", 1);

	return 0;
}