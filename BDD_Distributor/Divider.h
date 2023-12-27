#pragma once
#include "Module.h"
#include "Node.h"
#include <iostream>

struct NodeMap
{
    Node* node;
    Module* module;
};

class Divider
{
protected:
    bool flag = false;
public:
    virtual void divideModules(std::unordered_map<std::string, Module*>* modules, std::vector<Node*>* nodes, std::vector<NodeMap*>*& nodeMaps) {
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
    void divideModules(std::unordered_map<std::string, Module*>* modules, std::vector<Node*>* nodes, std::vector<NodeMap*>*& nodeMaps) override {
        
        Divider::divideModules(modules, nodes, nodeMaps);

        if (this->flag) { return; }

        int nodeUsed = 0;              

        for (auto& pair : *modules) {
            nodeMaps->push_back(new NodeMap{nodes->at(nodeUsed), pair.second});
            nodeUsed = (nodeUsed + 1) % nodes->size();
        }
    }
};

