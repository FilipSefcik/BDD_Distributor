#include "PlaManager.h"

PlaManager::PlaManager(std::string path)
{
	this->loadFile(path);
}

void PlaManager::loadFile(std::string path)
{
    // Utils:
    auto constexpr is_space = [](auto const character)
        {
            return static_cast<bool>(std::isspace(character));
        };

    auto constexpr to_words = [is_space](auto const str)
        {
            auto words = std::vector<std::string>();
            auto strIt = begin(str);
            auto const endIt = end(str);

            while (strIt != endIt)
            {
                auto const wordBegin = std::find_if_not(strIt, endIt, is_space);
                auto const wordEnd = std::find_if(wordBegin, endIt, is_space);
                if (wordBegin != wordEnd)
                {
                    words.emplace_back(wordBegin, wordEnd);
                }
                strIt = wordEnd;
            }

            return words;
        };

    auto ifst = std::ifstream(path);

    if (not ifst.is_open())
    {
        return;
    }

    // Read options.
    auto options = std::unordered_map<std::string, std::string>();
    auto line = std::string();
    while (std::getline(ifst, line))
    {
        // Skip leading spaces.
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

        if (*first != '.')
        {
            // Not an option.
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
            options.emplace(std::move(key), std::move(val));
        }
        else
        {
            options.emplace(std::move(key), std::string());
        }
    }

    // Parse header.
    auto const optionsEnd = std::end(options);
    auto const varCountIt = options.find(".i");
    auto const fCountIt = options.find(".o");
    auto const lineCountIt = options.find(".p");
    if (varCountIt == optionsEnd || fCountIt == optionsEnd
        || lineCountIt == optionsEnd)
    {
        return;
    }

    // Save values to atributes
    this->varCount = std::stoi(varCountIt->second);
    this->fCount = std::stoi(fCountIt->second);
    this->lineCount = std::stoi(lineCountIt->second);

    if (not varCount || not fCount || not lineCount)
    {
        return;
    }

    // Read data.
    this->lines.reserve(this->lineCount);
    do
    {
        auto const first
            = std::find_if_not(std::begin(line), std::end(line), is_space);
        auto const last = std::end(line);
        if (first == last)
        {
            // Skip empty line.
            continue;
        }

        if (*first == '.')
        {
            // This can only be the .e line.
            break;
        }

        //// Split on the first space.
        //auto const varsLast = std::find_if(first, last, is_space);
        //if (varsLast == last)
        //{
        //    return;
        //}
        //auto const fsFirst = std::find_if_not(varsLast + 1, last, is_space);
        //auto const fsLast = std::find_if(fsFirst, last, is_space);
        //auto const varsStr = std::string(first, varsLast);
        //auto const fStr = std::string(fsFirst, fsLast);

        //if (ssize(varsStr) != this->varCount || ssize(fStr) != this->fCount)
        //{
        //    return;
        //}

        this->lines.push_back(line);

    } while (std::getline(ifst, line));

}

int32_t PlaManager::getLineCount()
{
    return this->lineCount;
}

void PlaManager::printLines()
{
    for (std::string line : this->lines)
    {
        std::cout << line << std::endl;
    }
}
