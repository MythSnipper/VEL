#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <cstdint>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <limits>

#include <parser.hpp>


struct TypeCast : Expression{
    std::unique_ptr<Expression> Expr;
    Type Target;

    TypeCast(){};
    TypeCast(std::unique_ptr<Expression> expr, Type target){
        Expr = std::move(expr);
        Target = target;
    }
};

namespace SemanticAnalyzer{
    void analyze(Program& AST);
    void checkProgram(Program& AST);

    void checkGlobalVariableDeclaration(GlobalVariableDeclaration* decl, SymbolTable* parentTable);
    void checkFunctionDeclaration(Function* func, SymbolTable* parentTable);

    void checkFunction(Function* func, SymbolTable* parentTable);

    void checkStatement(Statement* statement, SymbolTable* parentTable, Function* parentFunction);
    void checkBlock(Block* block, SymbolTable* parentTable, Function* parentFunction);
    void checkVariableDeclaration(VariableDeclaration* decl, SymbolTable* parentTable);
    void checkReturn(Return* ret, SymbolTable* parentTable, Function* parentFunction);
    void checkIf(If* ifStatement, SymbolTable* parentTable, Function* parentFunction);
    void checkWhile(While* whileStatement, SymbolTable* parentTable, Function* parentFunction);
    void checkFor(For* forStatement, SymbolTable* parentTable, Function* parentFunction);
    void checkAssembly(Assembly* assembly, SymbolTable* parentTable);
    Type checkExpression(Expression* expr, SymbolTable* parentTable);



    Type checkPostfixOperatorType(PostfixOperator op, const Type& exprType);
    Type checkPrefixOperatorType(PrefixOperator op, const Type& exprType);
    Type checkBinaryOperatorType(BinaryOperator op, const Type& type1, const Type& type2);
    Type checkAssignmentOperatorType(BinaryOperator op, const Type& type1, const Type& type2);

    BuiltinType numericPromotion(const BuiltinType t1, const BuiltinType t2);
    bool checkExpressionAssignable(Expression* expr);

    Symbol* lookupSymbol(const std::string& id, SymbolTable* currTable);

    BuiltinType findBestLiteralType(int64_t vel);
    BuiltinType findBestLiteralType(double vel);

    bool checkTypeConversion(const Type& startType, const Type& endType, uint32_t* dangerLevel = nullptr);




}





#endif