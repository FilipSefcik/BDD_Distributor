#include "Module.h"

Module::Module(std::string paName) {
    this->name = paName;
    this->sons_reliability = new std::vector<std::vector<double>>();
}

void Module::addSon(Module* newSon)
{
    newSon->setParent(this);
    this->addPriority(newSon->getPriority());
}

void Module::printSons()
{
    std::cout << "Var count: " << this->varCount << std::endl;
    std::cout << "Priority: " << this->priority << std::endl;

    for (int i = 0; i < this->sons_reliability->size(); i++)
    {
        for (int j = 0; j < this->sons_reliability->at(i).size(); j++)
        {
            std::cout << this->sons_reliability->at(i).at(j) << " ";
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
        this->sons_reliability->push_back({0.5, 0.5});
    }
}

void Module::setSonsReliability(int sonPosition, double sonRel) {
    if (sonPosition >= 0 && sonPosition < this->sons_reliability->size()) {
        this->sons_reliability->at(sonPosition) = {1.0 - sonRel, sonRel};
    }
}

void Module::writePLAFile() {

    std::filesystem::path directoryPath = std::filesystem::path(this->path).parent_path();

    if (!std::filesystem::exists(directoryPath)) {
        if (!std::filesystem::create_directories(directoryPath)) {
            std::cerr << "Error creating directories!" << std::endl;
            return; // Return an error code
        }
    }

    std::ofstream outputFile(this->path);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return; 
    }

    outputFile << this->pla_file << std::endl;

    outputFile.close();
}

void Module::printSonsReliabilities() {
    for (int i = 0; i < this->sons_reliability->size(); i++) {
        for (int j = 0; j < this->sons_reliability->at(i).size(); j++) {
            std::cout << this->sons_reliability->at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
}
