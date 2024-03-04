// #pragma once
// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
// #include <unordered_map>
// #include <algorithm>
// /**
// * @brief Loads and save information from .pla file
// */
// class PlaManager
// {
// private:
// 	int32_t varCount; // number of variables
// 	int32_t fCount; // number of outputs - should always be 1
// 	int64_t lineCount; // number of lines
// 	std::vector<std::string> lines;
// public:
// 	PlaManager(std::string path);
// 	PlaManager(int32_t paVarCount, int32_t paFCount, int64_t paLineCount, std::vector<std::string> paLines);
// 	PlaManager(const PlaManager& secondManager);
// 	void loadFile(std::string path);
// 	int32_t getLineCount();
// 	int32_t getVarCount();
// 	void printLines();
// 	void writeToFiles(std::string path, int fileCount); // writes multiple pla files into "PLA-Files/write" directory
// 	std::vector<PlaManager> divideToInstances(int instanceCount);
// };

