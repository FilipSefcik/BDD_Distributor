#pragma once
#include "Module.h"
#include <cstdint>
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

class VarCountDivider : public Divider {
    public:
        void divideModules(std::vector<Module*>* modules, std::vector<int>* nodes) override {
        
        Divider::divideModules(modules, nodes);

        if (this->flag) { return; }     

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

