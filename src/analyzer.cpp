#include <analyzer.hpp>







namespace Analyzer{
    void analyze(Program& AST){
        checkProgram(AST);
    }

    void checkProgram(Program& AST){
        AST.Table = std::make_unique<SymbolTable>(SymbolTable{});
        AST.Table->parent = nullptr;

        //first do globally
        for(std::unique_ptr<Declaration>& decl : AST.TopLevel){
            if(GlobalVariableDeclaration* globalVar = dynamic_cast<GlobalVariableDeclaration*>(decl.get())){
                //handle global variable declarations
                checkGlobalVariableDeclaration(globalVar, AST.Table.get());
            }
            else if(Function* func = dynamic_cast<Function*>(decl.get())){
                //handle functions
                checkFunctionDeclaration(func, AST.Table.get());
            }
            else if(Assembly* assembly = dynamic_cast<Assembly*>(decl.get())){
                
            }
        }
        //then do functions
        for(std::unique_ptr<Declaration>& decl : AST.TopLevel){
            if(Function* func = dynamic_cast<Function*>(decl.get())){
                //do function scope
                checkFunction(func, AST.Table.get());
            }
        }
    }

    void checkGlobalVariableDeclaration(GlobalVariableDeclaration* decl, SymbolTable* parentTable){
        std::string id = decl->Id->Text;
        if(parentTable->Symbols.find(id) != parentTable->Symbols.end()){
            std::cout << "velc: Semantic Analyzer: Global variable \"" << id << "\" redeclared\n";
            exit(1);
        }
        else{
            if(!decl->Typename.isBuiltin){
                std::cout << "velc: Semantic Analyzer: Non builtin variable type cannot be handled in checkGlobalVariableDeclaration\n";
                exit(1);
            }
            BuiltinType newType = {decl->Typename.builtinType};
            Symbol newSymbol = {SymbolType::Variable, id, newType};
            parentTable->Symbols.insert({id, newSymbol});
            std::cout << "GLOBAL VAR: " << id << "\n";
        }
    }
    void checkFunctionDeclaration(Function* func, SymbolTable* parentTable){
        std::string id = func->Id->Text;
        if(parentTable->Symbols.find(id) != parentTable->Symbols.end()){
            std::cout << "velc: Semantic Analyzer: Function \"" << id << "\" redeclared\n";
            exit(1);
        }
        else{
            if(!func->ReturnType.isBuiltin){
                std::cout << "velc: Semantic Analyzer: Non builtin function return type cannot be handled in checkFunctionDeclaration\n";
                exit(1);
            }
            BuiltinType newType = {func->ReturnType.builtinType};
            Symbol newSymbol = {SymbolType::Function, id, newType};
            //add function parameter types to the function definition
            for(std::pair<Type, std::unique_ptr<Identifier>>& param : func->Parameters){
                Type type = param.first;
                if(!type.isBuiltin){
                    std::cout << "velc: Semantic Analyzer: Non builtin function parameter type cannot be handled in checkFUnctionDeclaration\n";
                    exit(1);
                }
                newSymbol.ParamTypes.push_back(type.builtinType);
            }
            parentTable->Symbols.insert({id, newSymbol});
            std::cout << "FUNCTION: " << id << "\n";
        }
    }

    void checkFunction(Function* func, SymbolTable* parentTable){
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
                std::cout << "velc: Semantic Analyzer: Non builtin function parameter type cannot be handled in checkFunction\n";
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
            checkIf(ifStatement, parentTable, parentFunction);
        }
        else if(While* whileStatement = dynamic_cast<While*>(statement)){
            checkWhile(whileStatement, parentTable, parentFunction);
        }
        else if(For* forStatement = dynamic_cast<For*>(statement)){
            checkFor(forStatement, parentTable, parentFunction);
        }
        else if(Assembly* assembly = dynamic_cast<Assembly*>(statement)){
            checkAssembly(assembly, parentTable);
        }
        else if(Empty* vel = dynamic_cast<Empty*>(statement)){
            
        }
        else{
            std::cout << "Exception $2\n";
            exit(-1);
        }
    }
    void checkBlock(Block* block, SymbolTable* parentTable, Function* parentFunction){
        block->Table = std::make_unique<SymbolTable>(SymbolTable{});
        block->Table->parent = parentTable;
        
        for(std::unique_ptr<Statement>& statement : block->Statements){
            checkStatement(statement.get(), block->Table.get(), parentFunction);
        }
    }
    void checkVariableDeclaration(VariableDeclaration* decl, SymbolTable* parentTable){
        std::string id = decl->Id->Text;
        if(parentTable->Symbols.find(id) != parentTable->Symbols.end()){
            std::cout << "velc: Semantic Analyzer: Local variable \"" << id << "\" redeclared\n";
            exit(1);
        }
        else{
            if(!decl->Typename.isBuiltin){
                std::cout << "velc: Semantic Analyzer: Non builtin variable type cannot be handled in checkVariableDeclaration\n";
                exit(1);
            }
            BuiltinType newType = {decl->Typename.builtinType};
            Symbol newSymbol = {SymbolType::Variable, id, newType};
            parentTable->Symbols.insert({id, newSymbol});
            std::cout << "LOCAL VAR: " << id << "\n";
        }
    }
    void checkReturn(Return* ret, SymbolTable* parentTable, Function* parentFunction){
        //check if type of expr can be converted to function return type
        Expression* expr = ret->Expr.get();
        Type exprType = checkExpression(expr, parentTable);

        if(!checkTypeConversion(exprType, parentFunction->ReturnType)){
            std::cout << "velc: Semantic Analyzer: Return type " << toString(exprType.builtinType) << " can't be converted to function return type " << toString(parentFunction->ReturnType.builtinType) << "\n";
            exit(1);
        }
    }
    void checkIf(If* ifStatement, SymbolTable* parentTable, Function* parentFunction){
        //check If
        ifStatement->TableIf = std::make_unique<SymbolTable>(SymbolTable{});
        ifStatement->TableIf->parent = parentTable;
        Expression* ifCondition = ifStatement->Condition.get();
        Type ifConditionType = checkExpression(ifCondition, ifStatement->TableIf.get());
        Statement* ifBody = ifStatement->Body.get();

        if(!checkTypeConversion(ifConditionType, {true, BuiltinType::BOOL, ""})){
            std::cout << "velc: Semantic Analyzer: If condition type " << toString(ifConditionType.builtinType) << " can't be converted to a boolean value\n";
            exit(1);
        }
        checkStatement(ifBody, ifStatement->TableIf.get(), parentFunction);

        //check ElseIfs
        for(ElseIf& elseif : ifStatement->ElseIfs){
            elseif.Table = std::make_unique<SymbolTable>(SymbolTable{});
            elseif.Table->parent = parentTable;
            Expression* elseifCondition = elseif.Condition.get();
            Type elseifConditionType = checkExpression(elseifCondition, elseif.Table.get());
            Statement* elseifBody = elseif.Body.get();
            
            if(!checkTypeConversion(elseifConditionType, {true, BuiltinType::BOOL, ""})){
                std::cout << "velc: Semantic Analyzer: ElseIf condition type " << toString(elseifConditionType.builtinType) << " can't be converted to a boolean value\n";
                exit(1);
            }
            checkStatement(elseifBody, elseif.Table.get(), parentFunction);
        }

        //check Else, if it exists
        if(ifStatement->Else){
            ifStatement->TableElse = std::make_unique<SymbolTable>(SymbolTable{});
            ifStatement->TableElse->parent = parentTable;
            Statement* elseBody = ifStatement->Else.get();

            checkStatement(elseBody, ifStatement->TableElse.get(), parentFunction);
        }
    }
    void checkWhile(While* whileStatement, SymbolTable* parentTable, Function* parentFunction){
        whileStatement->Table = std::make_unique<SymbolTable>(SymbolTable{});
        whileStatement->Table->parent = parentTable;
        Expression* whileCondition = whileStatement->Condition.get();
        Type whileConditionType = checkExpression(whileCondition, whileStatement->Table.get());
        Statement* whileBody = whileStatement->Body.get();
        if(!checkTypeConversion(whileConditionType, {true, BuiltinType::BOOL, ""})){
            std::cout << "velc: Semantic Analyzer: While loop condition type " << toString(whileConditionType.builtinType) << " can't be converted to a boolean value\n";
            exit(1);
        }
        checkStatement(whileBody, whileStatement->Table.get(), parentFunction);

    }
    void checkFor(For* forStatement, SymbolTable* parentTable, Function* parentFunction){
        forStatement->Table = std::make_unique<SymbolTable>(SymbolTable{});
        forStatement->Table->parent = parentTable;
        //check initializer
        if(Expression* expr = dynamic_cast<Expression*>(forStatement->Initializer.get())){ //Expr
            checkExpression(expr, forStatement->Table.get());
        }
        else if(VariableDeclaration* decl = dynamic_cast<VariableDeclaration*>(forStatement->Initializer.get())){ //VarDecl
            //add to current scope
            std::string id = decl->Id->Text;
            if(forStatement->Table->Symbols.find(id) != forStatement->Table->Symbols.end()){
                std::cout << "velc: Semantic Analyzer: Local variable in for loop initializer \"" << id << "\" redeclared\n";
                exit(1);
            }
            else{
                if(!decl->Typename.isBuiltin){
                    std::cout << "velc: Semantic Analyzer: Non builtin variable type cannot be handled in checkFor\n";
                    exit(1);
                }
                BuiltinType newType = {decl->Typename.builtinType};
                Symbol newSymbol = {SymbolType::Variable, id, newType};
                forStatement->Table->Symbols.insert({id, newSymbol});
                std::cout << "LOCAL VAR(FOR-INITIALIZER): " << id << "\n";
            }
        }
        else if(!forStatement->Initializer){ //if nullptr
            std::cout << "nullptr for initializer\n";
        }
        else{
            std::cout << "Exception $1\n";
            exit(-1);
        }

        //check condition
        Expression* forCondition = forStatement->Condition.get();
        Type forConditionType = checkExpression(forCondition, forStatement->Table.get());
        if(!checkTypeConversion(forConditionType, {true, BuiltinType::BOOL, ""})){
            std::cout << "velc: Semantic Analyzer: For loop condition type " << toString(forConditionType.builtinType) << " can't be converted to a boolean value\n";
            exit(1);
        }
        
        //check modifier
        Expression* forModifier = forStatement->Modifier.get();
        checkExpression(forModifier, forStatement->Table.get());

        //check body
        Statement* forBody = forStatement->Body.get();
        checkStatement(forBody, forStatement->Table.get(), parentFunction);
    }
    void checkAssembly(Assembly* assembly, SymbolTable* parentTable){
        //check assembly

    }
    Type checkExpression(Expression* expr, SymbolTable* parentTable){
        //nullptr
        if(expr == nullptr){
            return {true, BuiltinType::VOID, ""};
        }

        //literal
        if(Literal* literal = dynamic_cast<Literal*>(expr)){
            switch(literal->Type){
                case LiteralType::INT: return {true, findBestLiteralType(literal->IntValue), ""};
                case LiteralType::FLOAT: return {true, findBestLiteralType(literal->FloatValue), ""};
                case LiteralType::CHAR: return {true, BuiltinType::CHAR, ""};
                case LiteralType::STRING: return {true, BuiltinType::STRING, ""};
            }
        }

        //identifier
        if(Identifier* id = dynamic_cast<Identifier*>(expr)){
            Symbol* symbol = lookupSymbol(id->Text, parentTable);
            if(!symbol || symbol->Segregator != SymbolType::Variable){
                std::cout << "velc: Semantic Analyzer: Undefined variable " << id->Text << "\n";
                exit(1);
            }
            return {true, symbol->Type, ""};
        }

        //Call
        if(Call* call = dynamic_cast<Call*>(expr)){
            Symbol* functionDefinition = lookupSymbol(call->Id->Text, parentTable);
            if(!functionDefinition || functionDefinition->Segregator != SymbolType::Function){
                std::cout << "velc: Semantic Analyzer: Undefined function " << call->Id->Text << "\n";
                exit(1);
            }

            //check number of arguments of function definition and call
            if(call->Arguments.size() != functionDefinition->ParamTypes.size()){
                std::cout << "velc: Semantic Analyzer: Function call to " << call->Id->Text << " doesn't have the correct number of arguments\n";
                exit(1);
            }
            for(uint32_t i=0;i<call->Arguments.size();i++){
                Type argType = checkExpression(call->Arguments[i].get(), parentTable);
                if(!argType.isBuiltin){
                    std::cout << "velc: Semantic Analyzer: Cannot handle non builtin function call argument type\n";
                    exit(1);
                }
                Type definitionArgType = {true, functionDefinition->ParamTypes[i], ""};
                if(argType.builtinType != definitionArgType.builtinType){
                    if(checkTypeConversion(argType, definitionArgType)){ //if argument type can be converted
                        //insert cast
                        call->Arguments[i] = std::make_unique<TypeCast>(TypeCast{std::move(call->Arguments[i]), definitionArgType});
                    }
                    else{
                        std::cout << "velc: Semantic Analyzer: Function call argument of type " << toString(argType.builtinType) << " can't be converted to type " << toString(definitionArgType.builtinType) << " in function definition\n";
                        exit(1);
                    }
                }
            }
            return {true, functionDefinition->Type, ""}; //return function return type
        }

        //postfix
        if(PostfixOp* op = dynamic_cast<PostfixOp*>(expr)){

        }

        //prefix
        if(PrefixOp* op = dynamic_cast<PrefixOp*>(expr)){

        }

        //binary
        if(BinaryOp* op = dynamic_cast<BinaryOp*>(expr)){

        }

        //assignment
        if(AssignmentOp* op = dynamic_cast<AssignmentOp*>(expr)){

        }

        return {true, BuiltinType::VOID, ""};
    }

    Symbol* lookupSymbol(const std::string& id, SymbolTable* currTable){
        if(!currTable){
            return nullptr;
        }
        while(currTable != nullptr){
            auto entry = currTable->Symbols.find(id);
            if(entry != currTable->Symbols.end()){
                return &entry->second;
            }
            currTable = currTable->parent;
        }
        return nullptr;
    }

    BuiltinType findBestLiteralType(int64_t vel){
        if(vel >= 0){ //check positive limits
            if(vel > INT32_MAX)return BuiltinType::INT64;
            if(vel > INT16_MAX)return BuiltinType::INT32;
            if(vel > INT8_MAX)return BuiltinType::INT16;
            return BuiltinType::INT8;
        }
        if(vel < 0){ //check negative limits
            if(vel < INT32_MIN)return BuiltinType::INT64;
            if(vel < INT16_MIN)return BuiltinType::INT32;
            if(vel < INT8_MIN)return BuiltinType::INT16;
            return BuiltinType::INT8;
        }
        return BuiltinType::INT64;
    }
    BuiltinType findBestLiteralType(double vel){
        if(std::abs(vel) <= std::numeric_limits<float>::max()){
            float f = static_cast<float>(vel);
            if(static_cast<double>(f) == vel){
                return BuiltinType::FLOAT32;
            }
        }
        return BuiltinType::FLOAT64;
    }


    bool checkTypeConversion(Type startType, Type endType){
        //checks if startType can be converted to endType
        if(!(startType.isBuiltin && endType.isBuiltin)){
            std::cout << "velc: Semantic Analyzer: Non builtin variable type cannot be handled in checkTypeConversion\n";
            exit(1);
        }

        using BT = BuiltinType;
        BT s = startType.builtinType;
        BT e = endType.builtinType;

        //same type allowed
        if(s == e){
            return true;
        }
        
        auto isInteger = [](BT t){
            return t == BT::INT8 || t == BT::INT16 || t == BT::INT32 || t == BT::INT64 ||
                   t == BT::UINT8 || t == BT::UINT16 || t == BT::UINT32 || t == BT::UINT64;
        };
    
        auto isSignedInteger = [](BT t){
            return t == BT::INT8 || t == BT::INT16 || t == BT::INT32 || t == BT::INT64;
        };
    
        auto isUnsignedInteger = [](BT t){
            return t == BT::UINT8 || t == BT::UINT16 || t == BT::UINT32 || t == BT::UINT64;
        };
    
        auto isFloat = [](BT t){
            return t == BT::FLOAT32 || t == BT::FLOAT64;
        };

        //void to void allowed, trust
        if(s == BT::VOID || e == BT::VOID){
            std::cout << "velc: Semantic Analyzer: Warning: converting to/from void is more illegal than niko's hairline\n";
            return false;
        }

        // integer to integer
        if(isInteger(s) && isInteger(e)){
            // narrowing
            if((isSignedInteger(s) && isUnsignedInteger(e))){
                std::cout << "velc: Semantic Analyzer: Warning: conversion from signed to unsigned\n";
            }
            if((isUnsignedInteger(s) && isSignedInteger(e))){
                std::cout << "velc: Semantic Analyzer: Warning: conversion from unsigned to signed\n";
            }

            // smaller to larger is fine
            // larger to smaller is narrowing
            int sizeS = 0, sizeE = 0;
            switch(s){
                case BT::INT8: sizeS=8; break;
                case BT::INT16: sizeS=16; break;
                case BT::INT32: sizeS=32; break;
                case BT::INT64: sizeS=64; break;
                case BT::UINT8: sizeS=8; break;
                case BT::UINT16: sizeS=16; break;
                case BT::UINT32: sizeS=32; break;
                case BT::UINT64: sizeS=64; break;
                default: break;
            }
            switch(e){
                case BT::INT8: sizeE=8; break;
                case BT::INT16: sizeE=16; break;
                case BT::INT32: sizeE=32; break;
                case BT::INT64: sizeE=64; break;
                case BT::UINT8: sizeE=8; break;
                case BT::UINT16: sizeE=16; break;
                case BT::UINT32: sizeE=32; break;
                case BT::UINT64: sizeE=64; break;
                default: break;
            }
            if(sizeS > sizeE){
                std::cout << "velc: Semantic Analyzer: Warning: narrowing integer conversion from " << sizeS << " bits to " << sizeE << " bits\n";
            }

            return true;
        }

        //integer to float
        if(isInteger(s) && isFloat(e)){
            std::cout << "velc: Semantic Analyzer: Warning: converting integer to float may lose precision, like niko's sight without glasses\n";
            return true;
        }
        //float to integer
        if(isFloat(s) && isInteger(e)){
            std::cout << "velc: Semantic Analyzer: Warning: converting float to integer will truncate\n";
            return true;
        }
        //float to float
        if(isFloat(s) && isFloat(e)){
            int sizeS = (s==BT::FLOAT32?32:64);
            int sizeE = (e==BT::FLOAT32?32:64);
            if(sizeS > sizeE){
                std::cout << "velc: Semantic Analyzer: Warning: narrowing float conversion from " << sizeS << " bits to " << sizeE << " bits\n";
            }
            return true;
        }
        //boolean to something
        if(s == BT::BOOL && (isInteger(e) || isFloat(e))){
            std::cout << "velc: Semantic Analyzer: Warning: converting bool to numeric type\n";
            return true;
        }
        //something to boolean
        if((isInteger(s) || isFloat(s)) && e == BT::BOOL){
            std::cout << "velc: Semantic Analyzer: Warning: converting numeric type to bool\n";
            return true;
        }

        // char conversions
        if(s == BT::CHAR && (isInteger(e) || isFloat(e))){
            std::cout << "velc: Semantic Analyzer: Warning: converting char to numeric type\n";
            return true;
        }
        if((isInteger(s) || isFloat(s)) && e == BT::CHAR){
            std::cout << "velc: Semantic Analyzer: Warning: converting numeric type to char\n";
            return true;
        }

        if(s == BT::STRING && e != BT::BOOL){
            std::cout << "velc: Semantic Analyzer: Warning: conversion from string to non-boolean is more illegal than dio's hairline\n";
            return false;
        }

        std::cout << "velc: Semantic Analyzer: Warning: conversion from " << toString(s) << " to " << toString(e) << " is uncaught, disallowing by default.\n";
        return false;
    }


};





