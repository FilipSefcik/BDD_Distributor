#include "Module.h"

Module::~Module()
{
    /*for (auto& pair : this->sons)
    {
        delete pair.second;
    }*/

    this->sons.clear();
}

void Module::addSon(int position, Module* newSon)
{
    this->sons.emplace(position, newSon);
}

void Module::printSons()
{
    for (auto& pair : this->sons)
    {
        std::cout << pair.first << " " << pair.second->gatName() << std::endl;
    }
}
