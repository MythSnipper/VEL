#include <parser.hpp>

Program constructAST(const std::vector<Token>& tokenList){
    Program program;
    int index = 0;

    //construct program.TopLevel
    while(!isType(getToken(tokenList, index), TokenType::END_OF_FILE)){ //while current token is not EOF
        Token currentToken = getToken(tokenList, index);
        Token nextToken = getToken(tokenList, index, 1);
        Token nextnextToken = getToken(tokenList, index, 2);

        //Inline assembly
        if(isType(currentToken, TokenType::ASSEMBLY)){
            std::unique_ptr<Assembly> AssemblyNode = parseAssembly(tokenList, index);
            program.TopLevel.push_back(std::move(AssemblyNode));
        }
        else if(isTypename(currentToken) && isType(nextToken, TokenType::IDENTIFIER)){
            if(isType(nextnextToken, TokenType::LPAREN)){
                //Function
                std::unique_ptr<Function> FunctionNode = parseFunction(tokenList, index);
                program.TopLevel.push_back(std::move(FunctionNode));
            }
            else{
                //Variable Declaration
                std::unique_ptr<GlobalVariableDeclaration> GlobalVariableDeclarationNode = parseGlobalVariableDeclaration(tokenList, index);
                program.TopLevel.push_back(std::move(GlobalVariableDeclarationNode));
            }
        }
        else{
            std::cout << "Invalid token: " << currentToken.text << "\n";
            advance(tokenList, index, 1);
        }


    }

    return program;
};

//get a token with offset
Token getToken(const std::vector<Token>& tokenList, const int& index, const int& offset){
    return ((uint32_t)index < tokenList.size()-offset) ? tokenList[index+offset] : Token{TokenType::INVALID_TOKEN, "INVALID", 0, 0, 0, 0};
}

//advance
void advance(const std::vector<Token>&, int& index, const int& numTokens){
    index += numTokens;
}

bool isType(const Token& token, const TokenType& type){
    return token.type == type;
}

bool isTypename(const Token& token){
    switch(token.type){
        case TokenType::INT8_KEYWORD:
        case TokenType::INT16_KEYWORD:
        case TokenType::INT32_KEYWORD:
        case TokenType::INT64_KEYWORD:
        case TokenType::UINT8_KEYWORD:
        case TokenType::UINT16_KEYWORD:
        case TokenType::UINT32_KEYWORD:
        case TokenType::UINT64_KEYWORD:
        case TokenType::CHAR_KEYWORD:
        case TokenType::FLOAT32_KEYWORD:
        case TokenType::FLOAT64_KEYWORD:
        case TokenType::STRING_KEYWORD:
        case TokenType::VOID_KEYWORD:
            return true;
        default:
            return false;
    }
}

BuiltinType convertType(const TokenType& type){
    switch(type){
        case TokenType::INT8_KEYWORD:
            return BuiltinType::INT8;
        case TokenType::INT16_KEYWORD:
            return BuiltinType::INT16;
        case TokenType::INT32_KEYWORD:
            return BuiltinType::INT32;
        case TokenType::INT64_KEYWORD:
            return BuiltinType::INT64;
        case TokenType::UINT8_KEYWORD:
            return BuiltinType::UINT8;
        case TokenType::UINT16_KEYWORD:
            return BuiltinType::UINT16;
        case TokenType::UINT32_KEYWORD:
            return BuiltinType::UINT32;
        case TokenType::UINT64_KEYWORD:
            return BuiltinType::UINT64;
        case TokenType::FLOAT32_KEYWORD:
            return BuiltinType::FLOAT32;
        case TokenType::FLOAT64_KEYWORD:
            return BuiltinType::FLOAT64;
        case TokenType::CHAR_KEYWORD:
            return BuiltinType::CHAR;
        case TokenType::STRING_KEYWORD:
            return BuiltinType::STRING;
        case TokenType::VOID_KEYWORD:
            return BuiltinType::VOID;
        default:
            return BuiltinType::INT8;
    }
}

std::unique_ptr<Assembly> parseAssembly(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<Assembly> AssemblyNode = std::make_unique<Assembly>(Assembly{});
    AssemblyNode->Text = getToken(tokenList, index).text;

    std::cout << "\tAssembly parsed\nText: " << AssemblyNode->Text << "\n";

    advance(tokenList, index, 1); //skip assembly
    return AssemblyNode;
}

std::unique_ptr<Function> parseFunction(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<Function> FunctionNode = std::make_unique<Function>(Function{});
    FunctionNode->ReturnType = Type{true, convertType(getToken(tokenList, index).type), ""}; //First token is the return type
    
    FunctionNode->Id = std::make_unique<Identifier>(Identifier{getToken(tokenList, index, 1).text}); //Second token is the function identifier
    
    advance(tokenList, index, 2); //Skip return type, function identifier

    if(!isType(getToken(tokenList, index), TokenType::LPAREN)){ //Check if the token is LPAREN
        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
        throw std::runtime_error("Invalid token in parseFunction: Expected Left Parenthesis");
    }
    advance(tokenList, index, 1); //Skip LPAREN

    //Function parameters
    FunctionNode->Parameters = std::vector<std::pair<Type, std::unique_ptr<Identifier>>>();
    if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //There are parameters
        while(!isType(getToken(tokenList, index), TokenType::RPAREN)){
            Token currentToken = getToken(tokenList, index);
            Token nextToken = getToken(tokenList, index, 1);

            if(isTypename(currentToken) && isType(nextToken, TokenType::IDENTIFIER)){
                Type temptype{true, convertType(currentToken.type), ""};
                std::unique_ptr<Identifier> Id = std::make_unique<Identifier>(Identifier{nextToken.text});
                FunctionNode->Parameters.push_back({temptype, std::move(Id)});
                advance(tokenList, index, 2);

                if(isType(getToken(tokenList, index), TokenType::COMMA)){ //skip comma
                    advance(tokenList, index, 1);
                }
            }
            else{
                std::cout << "Info:\nCurrent token: " << currentToken.text << "Next token: " << nextToken.text << "\n";
                throw std::runtime_error("Invalid token in parseFunction: Expected typename and identifier");
            }
        }
    }

    //skip RPAREN
    if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
        throw std::runtime_error("Invalid token in parseFunction: Expected Right Parenthesis");
    }
    advance(tokenList, index, 1); //skip RPAREN

    //Function body
    if(isType(getToken(tokenList, index), TokenType::LBRACE)){ //Parse block
        FunctionNode->Body = parseBlock(tokenList, index);
    }
    else{
        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
        throw std::runtime_error("Invalid token in parseFunction: Expected Left Brace");
    }

    std::cout << "\tFunction parsed\nReturnType: " << (int)FunctionNode->ReturnType.builtinType << "\nId: " << FunctionNode->Id->Text << "\n";

    return FunctionNode;
}

std::unique_ptr<GlobalVariableDeclaration> parseGlobalVariableDeclaration(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<GlobalVariableDeclaration> GlobalVariableDeclarationNode = std::make_unique<GlobalVariableDeclaration>(GlobalVariableDeclaration{});
    GlobalVariableDeclarationNode->Typename = Type{true, convertType(getToken(tokenList, index).type), ""}; //First token is the return type
    
    GlobalVariableDeclarationNode->Id = std::make_unique<Identifier>(Identifier{getToken(tokenList, index, 1).text}); //Second token is the function identifier
    
    advance(tokenList, index, 2); //skip typename, identifier

    if(getToken(tokenList, index).type == TokenType::ASSIGN){
        advance(tokenList, index, 1); //skip assignment operator
        GlobalVariableDeclarationNode->Expr = parseExpression(tokenList, index); //parse expression till ;
    }
    else if(getToken(tokenList, index).type == TokenType::SEMICOLON){
        advance(tokenList, index, 1); //skip semicolon
        GlobalVariableDeclarationNode->Expr = nullptr;
    }
    else{
        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "\n";
        throw std::runtime_error("Invalid token in parseGlobalVariableDeclaration: Expected assignment operator or semicolon");
    }

    std::cout << "\tGlobalVariableDeclaration parsed\nTypename: " << (int)GlobalVariableDeclarationNode->Typename.builtinType << "\nId: " << GlobalVariableDeclarationNode->Id->Text << "\n";

    return GlobalVariableDeclarationNode;
}

std::unique_ptr<VariableDeclaration> parseVariableDeclaration(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<VariableDeclaration> VariableDeclarationNode = std::make_unique<VariableDeclaration>(VariableDeclaration{});
    VariableDeclarationNode->Typename = Type{true, convertType(getToken(tokenList, index).type), ""}; //First token is the return type
    
    VariableDeclarationNode->Id = std::make_unique<Identifier>(Identifier{getToken(tokenList, index, 1).text}); //Second token is the function identifier
    
    advance(tokenList, index, 2); //skip typename, identifier

    if(getToken(tokenList, index).type == TokenType::ASSIGN){
        advance(tokenList, index, 1); //skip assignment operator
        VariableDeclarationNode->Expr = parseExpression(tokenList, index); //parse expression till ;
    }
    else if(getToken(tokenList, index).type == TokenType::SEMICOLON){
        advance(tokenList, index, 1); //skip semicolon
        VariableDeclarationNode->Expr = nullptr;
    }
    else{
        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "\n";
        throw std::runtime_error("Invalid token in parseVariableDeclaration: Expected assignment operator or semicolon");
    }

    std::cout << "\tVariableDeclaration parsed\nTypename: " << (int)VariableDeclarationNode->Typename.builtinType << "\nId: " << VariableDeclarationNode->Id->Text << "\n";

    return VariableDeclarationNode;
}

std::unique_ptr<Block> parseBlock(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<Block> BlockNode = std::make_unique<Block>(Block{});
    BlockNode->Statements = std::vector<std::unique_ptr<Statement>>();
    
    //skip LBRACE
    if(!isType(getToken(tokenList, index), TokenType::LBRACE)){ //Check if the token is LBRACE
        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
        throw std::runtime_error("Invalid token in parseBlock: Expected Left Bracket");
    }
    advance(tokenList, index, 1); //skip LBRACE

    //Add statements
    while(!isType(getToken(tokenList, index), TokenType::RBRACE) && !isType(getToken(tokenList, index), TokenType::END_OF_FILE)){ //while current token is not }  and not EOF
        std::unique_ptr<Statement> statement = parseStatement(tokenList, index);
        BlockNode->Statements.push_back(std::move(statement));
    }
    //if while loop exited with EOF, error because no closing bracket
    if(isType(getToken(tokenList, index), TokenType::END_OF_FILE)){
        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
        throw std::runtime_error("Invalid token in parseBlock: Expected Right Bracket");
    }

    advance(tokenList, index, 1); //skip RBRACE

    return BlockNode;
}

std::unique_ptr<Statement> parseStatement(const std::vector<Token>& tokenList, int& index){
    //Parse statement
    Token currentToken = getToken(tokenList, index);

    if(isType(currentToken, TokenType::ASSEMBLY)){ //Parse Assembly
        return parseAssembly(tokenList, index);
    }
    if(isType(currentToken, TokenType::LBRACE)){ //Parse Block
        return parseBlock(tokenList, index);
    }
    if(isType(currentToken, TokenType::SEMICOLON)){ //Parse Empty
        advance(tokenList, index, 1); //skip semicolon
        return std::make_unique<Empty>();
    }
    if(isType(currentToken, TokenType::RETURN_KEYWORD)){ //Parse Return
        return parseReturn(tokenList, index);
    }
    if(isType(currentToken, TokenType::IF_KEYWORD)){ //Parse If
        return parseIf(tokenList, index);
    }
    if(isType(currentToken, TokenType::WHILE_KEYWORD)){ //Parse While
        return parseWhile(tokenList, index);
    }
    if(isType(currentToken, TokenType::FOR_KEYWORD)){ //Parse For
        return parseFor(tokenList, index);
    }

    //Parse Expression Statement is nothing matches
    return parseExpressionStatement(tokenList, index);

}

std::unique_ptr<Return> parseReturn(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<Return> returnNode = std::make_unique<Return>(Return{});
    advance(tokenList, index, 1); //skip return keyword
    returnNode->Expr = parseExpression(tokenList, index);

    //skip semicolon
    if(!isType(getToken(tokenList, index), TokenType::SEMICOLON)){ //Check if the token is SEMICOLON
        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
        throw std::runtime_error("Invalid token in parseReturn: Expected Semicolon");
    }
    advance(tokenList, index, 1); //skip SEMICOLON

    return returnNode;
}

std::unique_ptr<If> parseIf(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<If> ifNode = std::make_unique<If>(If{});
    return ifNode;
}

std::unique_ptr<While> parseWhile(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<While> whileNode = std::make_unique<While>(While{});
    return whileNode;
}

std::unique_ptr<For> parseFor(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<For> forNode = std::make_unique<For>(For{});
    return forNode;
}

std::unique_ptr<ExpressionStatement> parseExpressionStatement(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<ExpressionStatement> expressionStatementNode = std::make_unique<ExpressionStatement>(ExpressionStatement{});
    return expressionStatementNode;
}






std::unique_ptr<Expression> parseExpression(const std::vector<Token>& tokenList, int& index){
    std::unique_ptr<Expression> expressionNode = std::make_unique<Expression>(Expression{});
    return expressionNode;
}