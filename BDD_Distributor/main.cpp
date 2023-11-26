#include <iostream>
#include <libteddy/core.hpp>
#include "PlaManager.h"

int main() {
	PlaManager pla("PLA-Files/simplePLA.pla");
	std::cout << pla.getLineCount() << std::endl;
	pla.printLines();
	return 0;
}