#include "ModuleManager.h"
#include "Module.h"
#include <cstdint>
#include <vector>

void ModuleManager::load(std::string confPath) {
    this->loadModules(confPath);
    this->loadPLA();
}

void ModuleManager::loadModules(std::string confPath)
{
    auto constexpr is_space = [](auto const character)
        {
            return static_cast<bool>(std::isspace(character));
        };

    auto file = std::ifstream(confPath);

    if (not file.is_open())
    {
        printf("not file open error \n");
        return;
    }

    // Read paths to modules
    auto line = std::string();
    while (std::getline(file, line))
    {
        
        auto const first
            = std::find_if_not(std::begin(line), std::end(line), is_space);
        auto const last = std::end(line);
        if (first == last)
        {
            // Skip empty line.
            continue;
        }

        if (*first == '#')
        {
            // Skip comment.
            continue;
        }

        auto const filetype = line.substr(line.size() - 4);
        if (filetype != ".pla")
        {
            //not a path to a pla file
            break;
        }

        // Split into (key, val) pair on the first space.
        auto const keyLast = std::find_if(first, last, is_space);
        auto const valFirst = keyLast == last
            ? last
            : std::find_if_not(keyLast + 1, last, is_space);
        auto key = std::string(first, keyLast);
        if (valFirst != last)
        {
            auto valLast = last;
            while (is_space(*(valLast - 1)))
            {
                --valLast;
            }
            auto val = std::string(valFirst, valLast);
            Module* mod = new Module(std::move(key));
            mod->setPath(std::move(val));

            // size at start is 0, so when we add first module, it will have index 0, so it works
            this->module_mapping.emplace(mod->getName(), this->modules.size());
            this->modules.push_back(mod);

            // since we first added module, size is 1 but index is 0, so we need to discard 1
            // this->module_mapping.emplace(mod->getName(), this->vector_modules.size() - 1);
        }
    }

    // Read mapping
    do
    {
        auto const first
            = std::find_if_not(std::begin(line), std::end(line), is_space);
        auto const last = std::end(line);
        if (first == last || *first == '#')
        {
            // Skip empty line.
            continue;
        }

        auto const keyLast = std::find_if(first, last, is_space);
        auto const valFirst = keyLast == last
            ? last
            : std::find_if_not(keyLast + 1, last, is_space);
        auto key = std::string(first, keyLast);
        if (valFirst != last)
        {
            auto valLast = last;
            while (is_space(*(valLast - 1)))
            {
                --valLast;
            }
            auto val = std::string(valFirst, valLast);

            int digits = 0;
            int position;
            std::stringstream moduleName;
            for (std::size_t i = 0; i < val.size(); ++i) {
                if (val[i] == 'M') {
                    moduleName.str("");
                    moduleName << 'M';
                    position = i - digits;

                    for (size_t j = i + 1; j < val.size(); j++)
                    {
                        if (val[j] == 'V' || val[j] == 'M')
                        {
                            i = j - 1;
                            break;
                        }
                        moduleName << val[j];
                        digits++;
                    }
                    this->modules.at(this->module_mapping.at(moduleName.str()))->setPosition(position);
                    this->modules.at(this->module_mapping.at(key))->addSon(this->modules.at(this->module_mapping.at(moduleName.str())));
                }
            }
            this->modules.at(this->module_mapping.at(key))->setVarCount(val.size() - digits);
        }

    } while (std::getline(file, line));
}

ModuleManager::~ModuleManager()
{
    for (int i = 0; i < this->modules.size(); i++) {
        delete this->modules.at(i);
    }

    for (int i = 0; i < this->separate_instructions.size(); i++)
    {
        delete this->separate_instructions.at(i);
    }

    this->modules.clear();
    this->separate_instructions.clear();
}

void ModuleManager::printModules() {
    if (this->modules.empty()) { return; }

    for (Module* mod : this->modules) {
        std::cout << mod->getName() << " " << mod->getPath() << "\n";
        std::cout << "My position: " << mod->getPosition() << "\n";
        mod->printSons();
        std::cout << "\n";
    }
}

void ModuleManager::getInstructions(int nodesCount) {
    this->separate_instructions.resize(nodesCount > this->modules.size() ?
                                        this->modules.size() : nodesCount);

    std::sort(this->modules.begin(), this->modules.end(), [](Module* a, Module* b) { return a->getPriority() < b->getPriority(); });

    for (int i = 0; i < this->modules.size(); i++)
    {
        Module* mod = this->modules.at(i);
        Module* parent = mod->getParent();
        if (!this->separate_instructions.at(mod->getNodeRank()))
        {
            this->separate_instructions.at(mod->getNodeRank()) = new std::stringstream;
        }

        // EXEC - module name - position of the module in parent
        *this->separate_instructions.at(mod->getNodeRank()) << "EXEC " << mod->getName() << " " << mod->getPosition() << "\n";
        if (parent)
        {
            if (!this->separate_instructions.at(parent->getNodeRank()))
            {
                this->separate_instructions.at(parent->getNodeRank()) = new std::stringstream;
            }

            if (mod->getNodeRank() == parent->getNodeRank()) { 
                // LINK - name of parent module - name of son module
                *this->separate_instructions.at(mod->getNodeRank()) << "LINK " << parent->getName() << " " << mod->getName() << "\n";               
            } else {
                // SEND - name of module - rank of the process to send
                *this->separate_instructions.at(mod->getNodeRank()) << "SEND " << mod->getName() << " " << parent->getNodeRank() << "\n";
            
                // RECV - parent module name - rank of the process recieved from
                *this->separate_instructions.at(parent->getNodeRank()) << "RECV " << parent->getName() << " " << mod->getNodeRank() << "\n";
            }
        }
        else
        {
            // END - module which gives answer
            *this->separate_instructions.at(mod->getNodeRank()) << "END " << mod->getName() << "\n";
        }
    }
}

std::string ModuleManager::getInstructionFor(int node_rank) {
    if (node_rank >= 0 && node_rank < this->separate_instructions.size()) {
        return this->separate_instructions.at(node_rank)->str();
    }
    return "INVALID RANK";
}


std::vector<Module*>* ModuleManager::getModulesForNode(int node_rank) {
    std::vector<Module*>* nodes_modules = new std::vector<Module*>();
    for (Module* mod : this->modules) {
        if (mod->getNodeRank() == node_rank) {
            nodes_modules->push_back(mod);
        }
    }
    return nodes_modules;
}

void ModuleManager::printAssignedNodes()
{
    if (this->modules.empty()) { return; }

    for (Module* mod : this->modules)
    {
        std::cout << mod->getName() << " " << mod->getNodeRank() << "\n";
    }
}

void ModuleManager::printSeparateInstructions()
{
    for (int i = 0; i < this->separate_instructions.size(); i++)
    {
        std::cout << "Node " << i << " instructions:\n";
        std::cout << this->separate_instructions.at(i)->str() << "\n";
    }
}

void ModuleManager::printModulePLA()
{
    for (Module* mod : this->modules)
    {
        std::cout << mod->getName() << std::endl;
        mod->printPLA();
        std::cout << std::endl;
    }
}

void ModuleManager::loadPLA()
{
    if (this->modules.empty()) { return; }

    for (Module* mod : this->modules)
    {
        auto file = std::ifstream(mod->getPath());

        if (file.is_open())
        {
            std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            mod->setPLA(file_content);
        }
        
        file.close();

    }
}


