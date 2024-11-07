#pragma once
#include "module_manager.h"
#include <vector>

class merger {
  private:
    std::vector<module*>* modules;

  public:
    void set_modules(std::vector<module*>* pa_modules) { this->modules = pa_modules; };
    void merge_pla();
};