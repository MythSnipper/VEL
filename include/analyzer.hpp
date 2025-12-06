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
    void checkFunctionSymbols(Function* func, SymbolTable* parentTable);
    void checkStatementSymbols(Statement* statement, SymbolTable* parentTable, Function* parentFunction);
    



}





#endif