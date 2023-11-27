#include "PlaManager.h"

PlaManager::PlaManager(std::string path)
{
	this->loadFile(path);
}

PlaManager::PlaManager(int32_t paVarCount, int32_t paFCount, int64_t paLineCount, std::vector<std::string> paLines)
    : varCount(paVarCount), fCount(paFCount), lineCount(paLineCount), lines(paLines) {}

PlaManager::PlaManager(PlaManager& secondManager)
    : varCount(secondManager.varCount),
      fCount(secondManager.fCount),
      lineCount(secondManager.lineCount),
      lines(secondManager.lines) {}

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

int32_t PlaManager::getVarCount()
{
    return this->varCount;
}

void PlaManager::printLines()
{
    for (std::string line : this->lines)
    {
        std::cout << line << std::endl;
    }
}

void PlaManager::writeToFiles(std::string path, int fileCount)
{
    if (fileCount <= 0 || fileCount > this->lineCount)
    {
        std::cout << "Illegal file count.\n";
        return;
    }

    int fileNum = 0;
    int lineNum = this->lineCount / fileCount;
    int remainder = this->lineCount % fileCount;
    int firstLine = 0;

    do
    {
        std::ofstream file(path + "file" + std::to_string(fileNum) + ".pla");

        if (!file.is_open()) {
            std::cerr << "Error opening the file!" << std::endl;
            return; // Return an error code
        }

        int writtenLines = fileNum == 0
            ? lineNum + remainder
            : lineNum;

        file << ".i " << this->varCount << std::endl;
        file << ".o " << this->fCount << std::endl;
        file << ".p " << writtenLines << std::endl << std::endl;

        for (int i = 0; i < writtenLines; i++)
        {
            file << this->lines.at(firstLine + i) << std::endl;
        }

        file << ".e" << std::endl;
        file.close();

        firstLine += writtenLines;
        fileNum++;

    } while (fileNum < fileCount);
}

std::vector<PlaManager> PlaManager::divideToInstances(int instanceCount)
{
    std::vector<PlaManager> instances;

    if (instanceCount <= 0 || instanceCount > this->lineCount)
    {
        std::cout << "Illegal instance count.\n";
        return instances;
    }

    int instanceNum = 0;
    int lineNum = this->lineCount / instanceCount;
    int remainder = this->lineCount % instanceCount;
    int firstLine = 0;
    std::vector<std::string> newLines;
    std::string line;

    do
    {
        int writtenLines = instanceNum == 0
            ? lineNum + remainder
            : lineNum;

        for (int i = 0; i < writtenLines; i++)
        {
            line = this->lines.at(firstLine + i);
            newLines.emplace_back(line);
        }

        instances.push_back(PlaManager(this->varCount, this->fCount, writtenLines, newLines));
        
        newLines.clear();
        firstLine += writtenLines;
        instanceNum++;

    } while (instanceNum < instanceCount);


    return instances;
}
