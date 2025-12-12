#include <codegen.hpp>


namespace CodeGenerator{
    void generate(Program& AST, std::ostream& out){
        CodeGeneratorContext ctx{
            &out,
        };
        emitProgram(AST, ctx);

    }
    void emitProgram(Program& AST, CodeGeneratorContext& ctx){
        //loop through declarations
        for(std::unique_ptr<Declaration>& decl : AST.TopLevel){
            if(GlobalVariableDeclaration* globalVar = dynamic_cast<GlobalVariableDeclaration*>(decl.get())){
                emitGlobalVariableDeclaration(globalVar, ctx);
            }
            else if(Function* func = dynamic_cast<Function*>(decl.get())){
                emitFunction(func, ctx);
            }
            else if(Assembly* assembly = dynamic_cast<Assembly*>(decl.get())){
                emitAssembly(assembly, ctx);
            }
        }

    }
    void emitGlobalVariableDeclaration(GlobalVariableDeclaration* varDecl, CodeGeneratorContext& ctx){
        switch(varDecl->){

        }
    }
    void emitFunction(Function* func, CodeGeneratorContext& ctx){

    }
    void emitStatement(Statement* statement, CodeGeneratorContext& ctx){

    }
    void emitBlock(Block* block, CodeGeneratorContext& ctx){

    }
    void emitVariableDeclaration(VariableDeclaration* varDecl, CodeGeneratorContext& ctx){

    }
    void emitReturn(Return* ret, CodeGeneratorContext& ctx){

    }
    void emitIf(If* ifStatement, CodeGeneratorContext& ctx){

    }
    void emitWhile(While* whileStatement, CodeGeneratorContext& ctx){

    }
    void emitFor(For* forStatement, CodeGeneratorContext& ctx){

    }
    void emitAssembly(Assembly* inlineAsm, CodeGeneratorContext& ctx){

    }
    void emitExpression(Expression* expr, CodeGeneratorContext& ctx){

    }
    void emitTypeCast(TypeCast* cast, CodeGeneratorContext& ctx){

    }



}
















