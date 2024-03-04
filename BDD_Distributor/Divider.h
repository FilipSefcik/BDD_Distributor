#pragma once
#include "Module.h"
#include "Node.h"
#include <iostream>

class Divider
{
protected:
    bool flag = false;
public:
    virtual void divideModules(std::unordered_map<std::string, Module*>* modules, std::vector<Node*>* nodes) {
        if (modules->empty())
        {
            std::cout << "There are no modules to divide.\n";
            this->flag = true;
            return;
        }

        if (nodes->empty())
        {
            std::cout << "There are no nodes to divide for.\n";
            this->flag = true;
            return;
        }
    };
};

class NodeDivider : public Divider {
public:
    void divideModules(std::unordered_map<std::string, Module*>* modules, std::vector<Node*>* nodes) override {
        
        Divider::divideModules(modules, nodes);

        if (this->flag) { return; }

        int nodeUsed = 0;              

        for (auto& pair : *modules) {
            nodes->at(nodeUsed)->assignModule(pair.second);
            pair.second->assignNode(nodes->at(nodeUsed)->getRank());
            nodeUsed = (nodeUsed + 1) % nodes->size();
        }
    }
};

