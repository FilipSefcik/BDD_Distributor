#include "Module.h"

Module::~Module()
{
    for (auto& pair : this->sons)
    {
        delete pair.second;
    }

    this->sons.clear();
}
