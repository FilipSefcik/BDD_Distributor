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

    this->modules.clear();
}

void ModuleManager::printModules()
{
    if (this->modules.empty()) { return; }

    for (auto& pair : this->modules)
    {
        std::cout << pair.first << " " << pair.second->getPath() << "\n";
        pair.second->printSons();
    }
}
