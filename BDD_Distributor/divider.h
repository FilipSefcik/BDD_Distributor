#pragma once
#include "module.h"
#include <cstdint>

/*
 * @brief divider assigns modules from prameter to processing nodes
 *
 * Basic divider only checks wheter there are modules that can be divided.
 */
class divider {
  public:
    virtual void divide_modules(std::vector<module*>* modules, std::vector<module*> nodes[],
                                std::vector<int>* count) {
        if (modules->empty()) {
            std::cerr << "There are no modules to divide.\n";
            exit(3);
            return;
        }
    };
};

/*
 * @brief Divides modules among nodes evenly based on no particular property
 */
class node_divider : public divider {
  public:
    void divide_modules(std::vector<module*>* modules, std::vector<module*> nodes[],
                        std::vector<int>* count) override {

        divider::divide_modules(modules, nodes, count);

        int node_used = 0;

        for (module* mod : *modules) {
            nodes[node_used].push_back(mod);
            count->at(node_used)++;
            mod->assign_process(node_used);
            node_used = (node_used + 1) % count->size();
        }
    }
};

/*
 * @brief Divides modules among nodes evenly based on how many variables modules have.
 */
class var_count_divider : public divider {
  public:
    void divide_modules(std::vector<module*>* modules, std::vector<module*> nodes[],
                        std::vector<int>* count) override {

        divider::divide_modules(modules, nodes, count);

        std::vector<int> node_var_count;
        node_var_count.resize(count->size());

        std::sort(modules->begin(), modules->end(),
                  [](module* a, module* b) { return a->get_var_count() > b->get_var_count(); });

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
            nodes[node_used].push_back(mod);
            count->at(node_used)++;
            mod->assign_process(node_used);
        }
    }
};
