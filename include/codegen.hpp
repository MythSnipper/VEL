#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <cstdint>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <parser.hpp>
#include <analyzer.hpp>



struct CodeGeneratorContext{
    std::ostream* out;

};

namespace CodeGenerator{
    void generate(Program& AST, std::ostream& out);
    void emitProgram(Program* program, CodeGeneratorContext& ctx);
    void emitGlobalVariableDeclaration(GlobalVariableDeclaration* varDecl, CodeGeneratorContext& ctx);
    void emitFunction(Function* func, CodeGeneratorContext& ctx);
    void emitStatement(Statement* statement, CodeGeneratorContext& ctx);
    void emitBlock(Block* block, CodeGeneratorContext& ctx);
    void emitVariableDeclaration(VariableDeclaration* varDecl, CodeGeneratorContext& ctx);
    void emitReturn(Return* ret, CodeGeneratorContext& ctx);
    void emitIf(If* ifStatement, CodeGeneratorContext& ctx);
    void emitWhile(While* whileStatement, CodeGeneratorContext& ctx);
    void emitFor(For* forStatement, CodeGeneratorContext& ctx);
    void emitAssembly(Assembly* inlineAsm, CodeGeneratorContext& ctx);
    void emitExpression(Expression* expr, CodeGeneratorContext& ctx);
    void emitTypeCast(TypeCast* cast, CodeGeneratorContext& ctx);

}



#endif