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

/*
BuiltinType toBuiltinType(std::string type){

        case "int8": return BuiltinType::INT8;
        case "int16": return BuiltinType::INT16;
        case "int32": return BuiltinType::INT32;
        case "int64": return BuiltinType::INT64;
        case "uint8": return BuiltinType::UINT8;
        case "uint16": return BuiltinType::UINT16;
        case "uint32": return BuiltinType::UINT32;
        case "uint64": return BuiltinType::UINT64;
        case "float32": return BuiltinType::FLOAT32;
        case "float64": return BuiltinType::FLOAT64;
        case "char": return BuiltinType::CHAR;
        case "string": return BuiltinType::STRING;
        case "void": return BuiltinType::VOID;


    return BuiltinType::VOID;
}
*/


namespace Analyzer{
    void analyze(Program& AST){
        checkGlobalSymbols(AST);
    }

    void checkGlobalSymbols(Program& AST){
        AST.Table = std::make_unique<SymbolTable>(SymbolTable{});
        
        for(std::unique_ptr<Declaration>& decl : AST.TopLevel){
            if(GlobalVariableDeclaration* globalVar = dynamic_cast<GlobalVariableDeclaration*>(decl.get())){
                //handle global variable declarations
                std::string id = globalVar->Id->Text;
                if(AST.Table->Symbols.find(id) != AST.Table->Symbols.end()){
                    std::cout << "Identifier: " << id << "\n";
                    throw std::runtime_error("checkGlobalSymbols: Global variable redeclared");
                }
                else{
                    if(!globalVar->Typename.isBuiltin){
                        throw std::runtime_error("Non builtin variable type cannot be handled in checkGlobalSymbols");
                    }
                    BuiltinType newType = {globalVar->Typename.builtinType};
                    Symbol newSymbol = {SymbolType::Variable, id, newType};
                    AST.Table->Symbols.insert({id, newSymbol});
                    std::cout << "GLOBAL VAR: " << id << "\n";
                }
            }
            else if(Function* func = dynamic_cast<Function*>(decl.get())){
                //handle functions
                std::string id = func->Id->Text;
                if(AST.Table->Symbols.find(id) != AST.Table->Symbols.end()){
                    std::cout << "Identifier: " << id << "\n";
                    throw std::runtime_error("checkGlobalSymbols: Function redeclared");
                }
                else{
                    if(!func->ReturnType.isBuiltin){
                        throw std::runtime_error("Non builtin function return type cannot be handled in checkGlobalSymbols");
                    }
                    BuiltinType newType = {func->ReturnType.builtinType};
                    Symbol newSymbol = {SymbolType::Function, id, newType};
                    AST.Table->Symbols.insert({id, newSymbol});
                    std::cout << "FUNCTION: " << id << "\n";
                }
            }
        }
    }



};





