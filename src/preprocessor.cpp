#include <preprocessor.hpp>


namespace Preprocessor{
    
    std::string Process(const std::string& filepath, const std::vector<std::string>& includePaths){
        std::unordered_set<std::string> visited;
        return ProcessFile(filepath, includePaths, visited);
    }
    std::string ProcessFile(const std::string& filepath, const std::vector<std::string>& includePaths, std::unordered_set<std::string>& visitedPaths){
        if(visitedPaths.count(filepath)){
            std::cout << "velc: Preprocessor: Circular include detected: " << filepath << " already visited\n";
            exit(1);
        }
        visitedPaths.insert(filepath);

        //Open source file
        std::ifstream file;
        file.open(filepath);
        if(!file.is_open()){
            std::cout << "velc: Preprocessor: Failed to open file: " << filepath << "\n";
            exit(1);
        }

        std::string output;
        //read file by line and do preprocess
        std::string line; //line read from the file
        int vel; //length of preprocessor keyword + #
        std::string newfilepath; //new filename, used to call Process recursively
    
        while(std::getline(file, line)){
            if(line[0] == '#'){
                if(startsWith(line, "#include:")){ //include
                    vel = sizeof("#include:")-1; //exclude null terminator
                    newfilepath = line.substr(vel, line.size()-vel);

                    //remove leading spaces
                    /*
                    uint32_t start = 0;
                    while(start<newfilepath.size() && newfilepath[start] == ' '){
                        start++;
                    }
                    newfilepath = newfilepath.substr(start); */

                    newfilepath = findIncludeFile(newfilepath, includePaths);
                    output += ProcessFile(newfilepath, includePaths, visitedPaths);
                }
            }
            else{
                output += line+"\n";
            }
        }

        file.close();
        return output;
    }

    std::string findIncludeFile(const std::string& filepath, const std::vector<std::string>& includePaths){
        if(std::filesystem::exists(filepath)){
            return filepath;
        }
        for(const std::string& path : includePaths){
            std::filesystem::path vel = path;
            vel /= filepath;
            if(std::filesystem::exists(vel)){
                return vel.string();
            }
        }
        std::cout << "velc: Preprocessor: Couldn't find include file " << filepath << " in findIncludeFile\n";
        exit(1);
    }

    inline bool startsWith(const std::string& string, const std::string& substr){
        if(substr.size() > string.size()){
            return false;
        }
        return string.compare(0, substr.size(), substr) == 0;
    }
    inline bool endsWith(const std::string& string, const std::string& substr){
        if(substr.size() > string.size()){
            return false;
        }
        return string.compare(string.size()-substr.size(), substr.size(), substr) == 0;
    }

}