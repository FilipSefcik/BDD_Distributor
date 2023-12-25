#pragma once
#include <iostream>

class Divider
{
public:
    virtual void divideModules(int count) {
        std::cout << "Default Divider dividing..." << std::endl;
    };
};

class NodeDivider : public Divider {
public:
    void divideModules(int count) override {
        std::cout << "NodeDivider dividing..." << std::endl;
    }
};

