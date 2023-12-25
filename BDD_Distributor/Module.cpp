#include "Module.h"

Module::~Module()
{
    //not needed because it is the same pointer in class ModuleManager where it will be also deleted
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
    std::cout << "Var count: " << this->varCount << std::endl;
    for (auto& pair : this->sons)
    {
        std::cout << pair.first << " " << pair.second->gatName() << std::endl;
    }
}
