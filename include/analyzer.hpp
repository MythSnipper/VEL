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
    void checkStatement(Statement* statement, SymbolTable* parentTable, Function* parentFunction);
    void checkBlock(Block* block, SymbolTable* parentTable, Function* parentFunction);
    void checkVariableDeclaration(VariableDeclaration* block, SymbolTable* parentTable);
    void checkReturn(Return* block, SymbolTable* parentTable, Function* parentFunction);
    void checkIf(If* block, SymbolTable* parentTable);
    void checkWhile(While* block, SymbolTable* parentTable);
    void checkFor(For* block, SymbolTable* parentTable);
    void checkAssembly(Assembly* block, SymbolTable* parentTable);
    void checkExpression(Expression* expr, SymbolTable* parentTable);



}





#endif