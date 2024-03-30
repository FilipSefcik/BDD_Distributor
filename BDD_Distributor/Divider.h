#pragma once
#include "Module.h"
#include <cstdint>
#include <iostream>
#include <vector>

class Divider
{
public:
    virtual void divideModules(std::vector<Module*>* modules, std::vector<int>* nodes) {
        if (modules->empty())
        {
            std::cerr << "There are no modules to divide.\n";
            exit(3);
            return;
        }

        if (nodes->empty())
        {
            std::cerr << "There are no nodes to divide for.\n";
            exit(3);
            return;
        }
    };
};

class NodeDivider : public Divider {
public:
    void divideModules(std::vector<Module*>* modules, std::vector<int>* nodes) override {
        
        Divider::divideModules(modules, nodes);

        int nodeUsed = 0;              
        
        for (Module* mod : *modules) {
            nodes->at(nodeUsed)++;
            mod->assignNode(nodeUsed);
            nodeUsed = (nodeUsed + 1) % nodes->size();
        }
    }
};

class VarCountDivider : public Divider {
    public:
        void divideModules(std::vector<Module*>* modules, std::vector<int>* nodes) override {
        
        Divider::divideModules(modules, nodes);

        std::vector<int> node_var_count;
        node_var_count.resize(nodes->size());         

        std::sort(modules->begin(), modules->end(), [](Module* a, Module* b) { return a->getVarCount() > b->getVarCount(); });
        
        for (Module* mod : *modules) {
            int mod_var_count = mod->getVarCount();
            int nodeUsed = 0;
            for (int i = 0; i < node_var_count.size(); i++) {

                if (node_var_count.at(i) == 0) {
                    nodeUsed = i;
                    break;
                }

                if (node_var_count.at(nodeUsed) > node_var_count.at(i)) {
                    nodeUsed = i;
                }
            }
            node_var_count.at(nodeUsed) += mod_var_count;
            nodes->at(nodeUsed)++;
            mod->assignNode(nodeUsed);
        }
    }

};

