#include <analyzer.hpp>

/*
//Analyzer stuff
enum class SymbolType{
    Variable,
    Function,
    Parameter,
};
struct Symbol{
    SymbolType Segregator;
    std::string Id;
    BuiltinType Type;

    //may be used for later
    ASTNode* decl = nullptr;
};
struct SymbolTable{
    std::unordered_map<std::string, Symbol> Symbols;
    SymbolTable* parent = nullptr; //links to outer symbol table
};
*/



namespace Analyzer{
    void analyze(Program& AST){
        constructGlobalSymbolTable(AST);
    }
    void constructGlobalSymbolTable(Program& AST){
        for(std::unique_ptr<Declaration>& decl : AST.TopLevel){
            if(GlobalVariableDeclaration* globalVar = dynamic_cast<GlobalVariableDeclaration*>(decl.get())){

            }
            else if(Function* func = dynamic_cast<Function*>(decl.get())){

            }
        }
    }



};





