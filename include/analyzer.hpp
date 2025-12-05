#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <cstdint>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>


#include <parser.hpp>


namespace Analyzer{
    void analyze(Program& AST);
    void checkGlobalSymbols(Program& AST);
    


}





#endif