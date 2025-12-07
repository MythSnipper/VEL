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
        AST.Table->parent = nullptr;

        for(std::unique_ptr<Declaration>& decl : AST.TopLevel){
            if(GlobalVariableDeclaration* globalVar = dynamic_cast<GlobalVariableDeclaration*>(decl.get())){
                //handle global variable declarations
                std::string id = globalVar->Id->Text;
                if(AST.Table->Symbols.find(id) != AST.Table->Symbols.end()){
                    std::cout << "velc: Semantic Analyzer: Global variable \"" << id << "\" redeclared\n";
                    exit(1);
                }
                else{
                    if(!globalVar->Typename.isBuiltin){
                        std::cout << "velc: Semantic Analyzer: Non builtin variable type cannot be handled\n";
                        exit(1);
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
                    std::cout << "velc: Semantic Analyzer: Function \"" << id << "\" redeclared\n";
                    exit(1);
                }
                else{
                    if(!func->ReturnType.isBuiltin){
                        std::cout << "velc: Semantic Analyzer: Non builtin function return type cannot be handled\n";
                        exit(1);
                    }
                    BuiltinType newType = {func->ReturnType.builtinType};
                    Symbol newSymbol = {SymbolType::Function, id, newType};
                    AST.Table->Symbols.insert({id, newSymbol});
                    std::cout << "FUNCTION: " << id << "\n";
                    //do function scope
                    checkFunctionSymbols(func, AST.Table.get());
                }
            }
            else if(Assembly* assembly = dynamic_cast<Assembly*>(decl.get())){

            }
        }
    }
    void checkFunctionSymbols(Function* func, SymbolTable* parentTable){
        func->Table = std::make_unique<SymbolTable>(SymbolTable{});
        func->Table->parent = parentTable;

        for(std::pair<Type, std::unique_ptr<Identifier>>& param : func->Parameters){
            Type type = param.first;
            std::string id = param.second->Text;

            if(func->Table->Symbols.find(id) != func->Table->Symbols.end()){
                std::cout << "velc: Semantic Analyzer: Function parameter \"" << id << "\" redeclared in function " << func->Id->Text << " \n";
                exit(1);
            }
            if(!type.isBuiltin){
                std::cout << "velc: Semantic Analyzer: Non builtin function parameter type cannot be handled\n";
                exit(1);
            }
            BuiltinType newType = {type.builtinType};
            Symbol newSymbol;
            newSymbol.Segregator = SymbolType::Parameter;
            newSymbol.Id = id;
            newSymbol.Type = newType;
            func->Table->Symbols.insert({id, newSymbol});
            std::cout << "PARAMETER: " << id << "\n";

        }
        //do statement
        checkStatement(func->Body.get(), func->Table.get(), func);
    }
    void checkStatement(Statement* statement, SymbolTable* parentTable, Function* parentFunction){
        if(Block* block = dynamic_cast<Block*>(statement)){
            checkBlock(block, parentTable, parentFunction);
        }
        else if(VariableDeclaration* varDecl = dynamic_cast<VariableDeclaration*>(statement)){
            checkVariableDeclaration(varDecl, parentTable);
        }
        else if(Return* ret = dynamic_cast<Return*>(statement)){
            checkReturn(ret, parentTable, parentFunction);
        }
        else if(ExpressionStatement* exprStatement = dynamic_cast<ExpressionStatement*>(statement)){
            checkExpression(exprStatement->Expr.get(), parentTable);
        }
        else if(If* ifStatement = dynamic_cast<If*>(statement)){
            checkIf(ifStatement, parentTable);
        }
        else if(While* whileStatement = dynamic_cast<While*>(statement)){
            checkWhile(whileStatement, parentTable);
        }
        else if(For* forStatement = dynamic_cast<For*>(statement)){
            checkFor(forStatement, parentTable);
        }
        else if(Assembly* assembly = dynamic_cast<Assembly*>(statement)){
            checkAssembly(assembly, parentTable);
        }
        else if(Empty* vel = dynamic_cast<Empty*>(statement)){

        }
    }
    void checkBlock(Block* block, SymbolTable* parentTable, Function* parentFunction){
        block->Table = std::make_unique<SymbolTable>(SymbolTable{});
        block->Table->parent = parentTable;
        
        for(std::unique_ptr<Statement>& statement : block->Statements){
            checkStatement(statement.get(), parentTable, parentFunction);
        }
    }
    void checkVariableDeclaration(VariableDeclaration* block, SymbolTable* parentTable){

    }
    void checkReturn(Return* block, SymbolTable* parentTable, Function* parentFunction){

    }
    void checkIf(If* block, SymbolTable* parentTable){

    }
    void checkWhile(While* block, SymbolTable* parentTable){

    }
    void checkFor(For* block, SymbolTable* parentTable){

    }
    void checkAssembly(Assembly* block, SymbolTable* parentTable){

    }
    void checkExpression(Expression* expr, SymbolTable* parentTable){

    }



};





