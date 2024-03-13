#include "ModuleManager.h"

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
            this->modules.emplace(mod->getName(), mod);
            this->vector_modules.push_back(mod);
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
                    this->modules.at(key)->addSon(position, this->modules.at(moduleName.str()));
                    this->modules.at(moduleName.str())->setPosition(position);
                }
            }
            this->modules.at(key)->setVarCount(val.size() - digits);
        }

    } while (std::getline(file, line));
}

ModuleManager::~ModuleManager()
{
    for (auto& pair : this->modules)
    {
        delete pair.second;
    }

    for (int i = 0; i < this->separate_instructions.size(); i++)
    {
        delete this->separate_instructions.at(i);
    }

    this->modules.clear();
    this->separate_instructions.clear();
}

void ModuleManager::printModules()
{
    if (this->modules.empty()) { return; }

    for (auto& pair : this->modules)
    {
        std::cout << pair.first << " " << pair.second->getPath() << "\n";
        std::cout << "My position: " << pair.second->getPosition() << "\n";
        pair.second->printSons();
        std::cout << "\n";
    }
}

void ModuleManager::getInstructions(int nodesCount)
{
    this->separate_instructions.resize(nodesCount > modules.size() ?
                                        modules.size() : nodesCount);

    std::sort(this->vector_modules.begin(), this->vector_modules.end(), [](Module* a, Module* b) { return a->getPriority() < b->getPriority(); });

    for (int i = 0; i < this->vector_modules.size(); i++)
    {
        Module* mod = this->vector_modules.at(i);
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

void ModuleManager::printAssignedNodes()
{
    if (this->modules.empty()) { return; }

    for (auto& pair : this->modules)
    {
        std::cout << pair.first << " " << pair.second->getNodeRank() << "\n";
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

void ModuleManager::prinModulePLA()
{
    for (auto& pair : this->modules)
    {
        std::cout << pair.second->getName() << std::endl;
        pair.second->printPLA();
        std::cout << std::endl;
    }
}

void ModuleManager::loadPLA()
{
    if (this->modules.empty()) { return; }

    for (auto& pair : this->modules)
    {
        auto file = std::ifstream(pair.second->getPath());

        if (file.is_open())
        {
            std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            pair.second->setPLA(file_content);
        }
        
        file.close();

    }
}


