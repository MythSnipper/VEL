#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>


#include <preprocessor.hpp>
#include <lexer.hpp>
#include <parser.hpp>
#include <analyzer.hpp>



struct CompilerOptions{
    //info from argv
    std::string input_filename;
    std::string input_filename_base;
    std::string output_filename;

    std::vector<std::string> includePaths;
    
    std::string source;
    std::string sourceAssembly;
    std::vector<Token> tokens;
    Program AST;

    //options
    bool timeExecution = false;

    bool doPreprocess = true;
    bool doCompile = true;
    bool doAssemble = true;
    bool doLink = true;
    bool doExecute = false;

    bool deleteIntermediateFiles = true;
    //used only for argv parsing
    bool expectOutputFile = false;

};






std::string read_file(const std::string& path);
inline bool endsWith(const std::string& string, const std::string& extension);



#endif