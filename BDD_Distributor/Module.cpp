#include "Module.h"

void Module::addSon(int position, Module* newSon)
{
    newSon->setParent(this);
    this->addPriority(newSon->getPriority());
}

void Module::printSons()
{
    std::cout << "Var count: " << this->varCount << std::endl;
    std::cout << "Priority: " << this->priority << std::endl;

    for (int i = 0; i < this->sons_reliability.size(); i++)
    {
        for (int j = 0; j < this->sons_reliability.at(i).size(); j++)
        {
            std::cout << this->sons_reliability.at(i).at(j) << " ";
        }
        std::cout << std::endl;
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

void Module::setVarCount(int paVarCount) {
    this->varCount = paVarCount;
    for (int i = 0; i < this->varCount; i++)
    {
        this->sons_reliability.push_back({0.5, 0.5});
    }
}
