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
    newSon->setParent(this);
    this->sons.emplace(position, newSon);
    this->addPriority(newSon->getPriority());
}

void Module::printSons()
{
    std::cout << "Var count: " << this->varCount << std::endl;
    std::cout << "Priority: " << this->priority << std::endl;
    
    for (auto& pair : this->sons)
    {
        std::cout << pair.first << " " << pair.second->getName() << std::endl;
    }
}

void Module::addPriority(int sonPriority)
{
    if (this->priority == sonPriority)
    {
        this->priority++;
        if (this->parent) 
        {
            this->parent->addPriority(this->priority);
        }
    }
}

void Module::printPLA() {
    std::cout << this->pla_file << std::endl;
}
