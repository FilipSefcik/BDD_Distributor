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
            Module* mod = new Module(std::move(key), std::move(val));
            this->modules.emplace(mod->getName(), mod);
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

void ModuleManager::getInstructions(std::vector<Node*>* nodes)
{
    std::vector<Module*> instructions;

    this->separate_instructions.resize(nodes->size() > modules.size() ?
                                        modules.size() : nodes->size());

    for (auto& pair : this->modules)
    {
        instructions.push_back(pair.second);
    }

    std::sort(instructions.begin(), instructions.end(), [](Module* a, Module* b) { return a->getPriority() < b->getPriority(); });

    std::cout << "Sorted:\n";

    for (int i = 0; i < instructions.size(); i++)
    {
        std::cout << instructions.at(i)->getName() << " ";
    }
    std::cout << std::endl;

    std::stringstream written_instructions;

    for (int i = 0; i < instructions.size(); i++)
    {
        Module* mod = instructions.at(i);
        Module* parent = mod->getParent();
        if (!this->separate_instructions.at(mod->getNodeRank()))
        {
            this->separate_instructions.at(mod->getNodeRank()) = new std::stringstream;
        }

        written_instructions << "EXEC " << mod->getName() << "\n";
        *this->separate_instructions.at(mod->getNodeRank()) << "EXEC " << mod->getName() << "\n"; 
        if (parent)
        {
            if (!this->separate_instructions.at(parent->getNodeRank()))
            {
                this->separate_instructions.at(parent->getNodeRank()) = new std::stringstream;
            }

            written_instructions << "SEND " << parent->getNodeRank() << " " << mod->getPosition() << "\n";
            *this->separate_instructions.at(mod->getNodeRank()) << "SEND " << parent->getNodeRank() << " " << mod->getPosition() << "\n";

            written_instructions << "RECV " << parent->getName() << " " << mod->getNodeRank() << "\n";
            *this->separate_instructions.at(parent->getNodeRank()) << "RECV " << parent->getName() << " " << mod->getNodeRank() << "\n";
        }
    }

    std::cout << "Full instructions:\n";
    std::cout << written_instructions.str() << std::endl;
}

void ModuleManager::printAssignedNodes()
{
    if (this->modules.empty()) { return; }

    for (auto& pair : this->modules)
    {
        std::cout << pair.first << " " << pair.second->getNodeRank() << "\n";
    }
}

bool ModuleManager::comparator(Module* a, Module* b)
{
    return a->getPriority() < b->getPriority();
}

void ModuleManager::printSeparateInstructions()
{
    for (int i = 0; i < this->separate_instructions.size(); i++)
    {
        std::cout << "Node " << i << " instructions:\n";
        std::cout << this->separate_instructions.at(i)->str() << "\n";
    }
}
