#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_set>


namespace Preprocessor{
    std::string Process(const std::string& filepath, const std::vector<std::string>& includePaths);
    std::string ProcessFile(const std::string& filepath, const std::vector<std::string>& includePaths, std::unordered_set<std::string>& visitedPaths);
    std::string findIncludeFile(const std::string& filepath, const std::vector<std::string>& includePaths);
    inline bool startsWith(const std::string& string, const std::string& extension);
    inline bool endsWith(const std::string& string, const std::string& extension);
}





#endif