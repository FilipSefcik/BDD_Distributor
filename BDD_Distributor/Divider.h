#pragma once
#include "Module.h"
#include <iostream>
#include <vector>

class Divider
{
protected:
    bool flag = false;
public:
    virtual void divideModules(std::vector<Module*>* modules, std::vector<int>* nodes) {
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
    void divideModules(std::vector<Module*>* modules, std::vector<int>* nodes) override {
        
        Divider::divideModules(modules, nodes);

        if (this->flag) { return; }

        int nodeUsed = 0;              
        
        for (Module* mod : *modules) {
            nodes->at(nodeUsed)++;
            mod->assignNode(nodeUsed);
            nodeUsed = (nodeUsed + 1) % nodes->size();
        }
    }
};

