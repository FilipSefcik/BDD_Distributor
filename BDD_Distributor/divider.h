#pragma once
#include "module.h"
#include <cstdint>
#include <iostream>
#include <vector>

class divider
{
public:
    virtual void divide_modules(std::vector<module*>* modules, std::vector<int>* nodes) {
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

class node_divider : public divider {
public:
    void divide_modules(std::vector<module*>* modules, std::vector<int>* nodes) override {
        
        divider::divide_modules(modules, nodes);

        int node_used = 0;              
        
        for (module* mod : *modules) {
            nodes->at(node_used)++;
            mod->assign_process(node_used);
            node_used = (node_used + 1) % nodes->size();
        }
    }
};

class var_count_divider : public divider {
    public:
        void divide_modules(std::vector<module*>* modules, std::vector<int>* nodes) override {
        
        divider::divide_modules(modules, nodes);

        std::vector<int> node_var_count;
        node_var_count.resize(nodes->size());         

        std::sort(modules->begin(), modules->end(), [](module* a, module* b) { return a->get_var_count() > b->get_var_count(); });
        
        for (module* mod : *modules) {
            int mod_var_count = mod->get_var_count();
            int node_used = 0;
            for (int i = 0; i < node_var_count.size(); i++) {

                if (node_var_count.at(i) == 0) {
                    node_used = i;
                    break;
                }

                if (node_var_count.at(node_used) > node_var_count.at(i)) {
                    node_used = i;
                }
            }
            node_var_count.at(node_used) += mod_var_count;
            nodes->at(node_used)++;
            mod->assign_process(node_used);
        }
    }

};

