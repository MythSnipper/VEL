#include <parser.hpp>

std::unordered_map<TokenType, int> binding_power_table = {

    {TokenType::SWAP, 20},                 // $$

    {TokenType::MUL, 13},                 // *
    {TokenType::DIV, 13},                 // /
    {TokenType::MOD, 13},                 // %

    {TokenType::ADD, 12},                 // +
    {TokenType::SUB, 12},                 // -

    {TokenType::LSHIFT, 11},              // <<
    {TokenType::RSHIFT, 11},              // >>

    {TokenType::LT, 10},                  // <
    {TokenType::GT, 10},                  // >
    {TokenType::LTE, 10},                 // <=
    {TokenType::GTE, 10},                 // >=

    {TokenType::EQ, 9},                   // ==
    {TokenType::NEQ, 9},                  // != 

    {TokenType::AND, 8},                  // &
    {TokenType::XOR, 7},                  // ^
    {TokenType::OR, 6},                   // |
    
    {TokenType::ANDAND, 5},               // &&
    {TokenType::OROR, 4},                 // ||
    {TokenType::XORXOR, 3},               // ^^

    {TokenType::ASSIGN, 1},               // =
    {TokenType::ADD_ASSIGN, 1},           // +=
    {TokenType::SUB_ASSIGN, 1},           // -=
    {TokenType::MUL_ASSIGN, 1},           // *=
    {TokenType::DIV_ASSIGN, 1},           // /=
    {TokenType::MOD_ASSIGN, 1},           // %=
    {TokenType::AND_ASSIGN, 1},           // &=
    {TokenType::OR_ASSIGN, 1},            // |=
    {TokenType::XOR_ASSIGN, 1},           // ^=
    {TokenType::LSHIFT_ASSIGN, 1},        // <<=
    {TokenType::RSHIFT_ASSIGN, 1},        // >>=
    {TokenType::NOT_ASSIGN, 1},           // !~=
    
};
std::unordered_map<TokenType, int> binding_power_table_postfix = {
    //POSTFIX
    {TokenType::INC, 14},                 // ++
    {TokenType::DEC, 14},                 // -- 
};
std::unordered_map<TokenType, int> binding_power_table_prefix = {
    //PREFIX
    {TokenType::NOT_LOGICAL, 15},         // !
    {TokenType::NOT_BITWISE, 15},         // !~
    {TokenType::INC, 15},                 // ++
    {TokenType::DEC, 15},                 // --
    {TokenType::ADD, 15},                 // +
    {TokenType::SUB, 15},                 // -
};


//stuff for debug print in header file
void printIndent(int level){
    for(int i=0;i<level;i++){
        std::cout << "    ";
    }
}
std::string toString(BuiltinType t){
    switch(t){
        case BuiltinType::BOOL: return "bool";
        case BuiltinType::INT8: return "int8";
        case BuiltinType::INT16: return "int16";
        case BuiltinType::INT32: return "int32";
        case BuiltinType::INT64: return "int64";
        case BuiltinType::UINT8: return "uint8";
        case BuiltinType::UINT16: return "uint16";
        case BuiltinType::UINT32: return "uint32";
        case BuiltinType::UINT64: return "uint64";
        case BuiltinType::FLOAT32: return "float32";
        case BuiltinType::FLOAT64: return "float64";
        case BuiltinType::CHAR: return "char";
        case BuiltinType::STRING: return "string";
        case BuiltinType::VOID: return "void";
    }
    return "nein";
}
std::string toString(LiteralType t){
    switch(t){
        case LiteralType::INT: return "int";
        case LiteralType::FLOAT: return "float";
        case LiteralType::CHAR: return "char";
        case LiteralType::STRING: return "string";
    }
    return "nein";
}
std::string toString(PostfixOperator t){
    switch(t){
        case PostfixOperator::INC: return "++";
        case PostfixOperator::DEC: return "--";
    }
    return "nein";
}
std::string toString(PrefixOperator t){
    switch(t){
        case PrefixOperator::LOGICAL_NOT: return "!";
        case PrefixOperator::BITWISE_NOT: return "!~";
        case PrefixOperator::INC: return "++";
        case PrefixOperator::DEC: return "--";
        case PrefixOperator::ADD: return "+";
        case PrefixOperator::NEG: return "-";
    }
    return "nein";
}
std::string toString(BinaryOperator t){
    switch(t){
        case BinaryOperator::ADD: return "+";
        case BinaryOperator::SUB: return "-";
        case BinaryOperator::MUL: return "*";
        case BinaryOperator::DIV: return "/";
        case BinaryOperator::MOD: return "%";

        case BinaryOperator::EQ: return "==";
        case BinaryOperator::NEQ: return "!=";
        case BinaryOperator::LT: return "<";
        case BinaryOperator::GT: return ">";
        case BinaryOperator::LTE: return "<=";
        case BinaryOperator::GTE: return ">=";

        case BinaryOperator::BITWISE_AND: return "&";
        case BinaryOperator::BITWISE_OR: return "|";
        case BinaryOperator::BITWISE_XOR: return "^";
        case BinaryOperator::LSHIFT: return "<<";
        case BinaryOperator::RSHIFT: return ">>";

        case BinaryOperator::LOGICAL_AND: return "&&";
        case BinaryOperator::LOGICAL_OR: return "||";
        case BinaryOperator::LOGICAL_XOR: return "^^";

        case BinaryOperator::ASSIGN: return "=";
        case BinaryOperator::ADD_ASSIGN: return "+=";
        case BinaryOperator::SUB_ASSIGN: return "-=";
        case BinaryOperator::MUL_ASSIGN: return "*=";
        case BinaryOperator::DIV_ASSIGN: return "/=";
        case BinaryOperator::MOD_ASSIGN: return "%=";
        case BinaryOperator::AND_ASSIGN: return "&=";
        case BinaryOperator::OR_ASSIGN: return "|=";
        case BinaryOperator::XOR_ASSIGN: return "^=";
        case BinaryOperator::LSHIFT_ASSIGN: return "<<=";
        case BinaryOperator::RSHIFT_ASSIGN: return ">>=";
        case BinaryOperator::NOT_ASSIGN: return "!~=";
        case BinaryOperator::SWAP: return "$$";
    }
    return "nein";
}


namespace Parser{
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
            case TokenType::BOOL_KEYWORD:
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

    bool isLiteral(const Token& token){
        switch(token.type){
            case TokenType::INT_LITERAL:
            case TokenType::FLOAT_LITERAL:
            case TokenType::CHAR_LITERAL:
            case TokenType::STRING_LITERAL:
                return true;
            default:
                return false;
        }
    }

    BuiltinType toBuiltinType(const TokenType& type){
        switch(type){
            case TokenType::BOOL_KEYWORD:
                return BuiltinType::BOOL;
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

        advance(tokenList, index, 1); //skip assembly
        return AssemblyNode;
    }

    std::unique_ptr<Function> parseFunction(const std::vector<Token>& tokenList, int& index){
        std::unique_ptr<Function> FunctionNode = std::make_unique<Function>(Function{});
        FunctionNode->ReturnType = Type{true, toBuiltinType(getToken(tokenList, index).type), ""}; //First token is the return type
        
        FunctionNode->Id = std::make_unique<Identifier>(Identifier{getToken(tokenList, index, 1).text}); //Second token is the function identifier
        
        advance(tokenList, index, 2); //Skip return type, function identifier

        if(!isType(getToken(tokenList, index), TokenType::LPAREN)){ //Check if the token is LPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseFunction: Expected Left Parenthesis\n";
            exit(1);
        }
        advance(tokenList, index, 1); //Skip LPAREN

        //Function parameters
        FunctionNode->Parameters = std::vector<std::pair<Type, std::unique_ptr<Identifier>>>();
        if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //There are parameters
            while(!isType(getToken(tokenList, index), TokenType::RPAREN)){
                Token currentToken = getToken(tokenList, index);
                Token nextToken = getToken(tokenList, index, 1);

                if(isTypename(currentToken) && isType(nextToken, TokenType::IDENTIFIER)){
                    Type temptype{true, toBuiltinType(currentToken.type), ""};
                    std::unique_ptr<Identifier> Id = std::make_unique<Identifier>(Identifier{nextToken.text});
                    FunctionNode->Parameters.push_back({temptype, std::move(Id)});
                    advance(tokenList, index, 2);

                    if(isType(getToken(tokenList, index), TokenType::COMMA)){ //skip comma
                        advance(tokenList, index, 1);
                    }
                }
                else{
                    std::cout << "Info:\nCurrent token: " << currentToken.text << "Next token: " << nextToken.text << "\n";
                    std::cout << "velc: Parser: Invalid token in parseFunction: Expected typename and identifier\n";
                    exit(1);
                }
            }
        }

        //skip RPAREN
        if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseFunction: Expected Right Parenthesis\n";
            exit(1);
        }
        advance(tokenList, index, 1); //skip RPAREN

        //Function body
        FunctionNode->Body = parseStatement(tokenList, index);

        return FunctionNode;
    }

    std::unique_ptr<GlobalVariableDeclaration> parseGlobalVariableDeclaration(const std::vector<Token>& tokenList, int& index){
        std::unique_ptr<GlobalVariableDeclaration> GlobalVariableDeclarationNode = std::make_unique<GlobalVariableDeclaration>(GlobalVariableDeclaration{});
        GlobalVariableDeclarationNode->Typename = Type{true, toBuiltinType(getToken(tokenList, index).type), ""}; //First token is the return type
        
        GlobalVariableDeclarationNode->Id = std::make_unique<Identifier>(Identifier{getToken(tokenList, index, 1).text}); //Second token is the function identifier
        
        advance(tokenList, index, 2); //skip typename, identifier

        if(isType(getToken(tokenList, index), TokenType::ASSIGN)){
            advance(tokenList, index, 1); //skip assignment operator
            GlobalVariableDeclarationNode->Expr = parseExpression(tokenList, index); //parse expression till ;
            //skip Semicolon
            if(!isType(getToken(tokenList, index), TokenType::SEMICOLON)){ //Check if the token is SEMICOLON
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::cout << "velc: Parser: Invalid token in parseGlobalVariableDeclaration: Expected Semicolon\n";
                exit(1);
            }
            advance(tokenList, index, 1); //skip SEMICOLON
        }
        else if(isType(getToken(tokenList, index), TokenType::SEMICOLON)){
            advance(tokenList, index, 1); //skip semicolon
            GlobalVariableDeclarationNode->Expr = nullptr;
        }
        else{
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseGlobalVariableDeclaration: Expected assignment operator or semicolon\n";
            exit(1);
        }

        return GlobalVariableDeclarationNode;
    }

    std::unique_ptr<VariableDeclaration> parseVariableDeclaration(const std::vector<Token>& tokenList, int& index, const TokenType& stopType){
        std::unique_ptr<VariableDeclaration> VariableDeclarationNode = std::make_unique<VariableDeclaration>(VariableDeclaration{});
        VariableDeclarationNode->Typename = Type{true, toBuiltinType(getToken(tokenList, index).type), ""}; //First token is the return type
        
        VariableDeclarationNode->Id = std::make_unique<Identifier>(Identifier{getToken(tokenList, index, 1).text}); //Second token is the function identifier
        
        advance(tokenList, index, 2); //skip typename, identifier

        if(isType(getToken(tokenList, index), TokenType::ASSIGN)){
            advance(tokenList, index, 1); //skip assignment operator
            VariableDeclarationNode->Expr = parseExpression(tokenList, index); //parse expression till stop token
            //skip stop token
            if(!isType(getToken(tokenList, index), stopType)){ //Check if the token is stop token
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::cout << "Invalid token in parseVariableDeclaration: Expected ";
                std::cout << ((stopType == TokenType::SEMICOLON) ? "Semicolon" : "Stop token");
                exit(1);
            }
            advance(tokenList, index, 1); //skip stop token
        }
        else if(isType(getToken(tokenList, index), stopType)){
            advance(tokenList, index, 1); //skip stop token
            VariableDeclarationNode->Expr = nullptr;
        }
        else{
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseVariableDeclaration: Expected assignment operator or semicolon\n";
            exit(1);
        }

        return VariableDeclarationNode;
    }

    std::unique_ptr<Block> parseBlock(const std::vector<Token>& tokenList, int& index){
        std::unique_ptr<Block> BlockNode = std::make_unique<Block>(Block{});
        BlockNode->Statements = std::vector<std::unique_ptr<Statement>>();
        
        //skip LBRACE
        if(!isType(getToken(tokenList, index), TokenType::LBRACE)){ //Check if the token is LBRACE
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseBlock: Expected Left Bracket\n";
            exit(1);
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
            std::cout << "velc: Parser: Invalid token in parseBlock: Expected Right Bracket\n";
            exit(1);
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

        Token nextToken = getToken(tokenList, index, 1);
        if(isTypename(currentToken) && isType(nextToken, TokenType::IDENTIFIER)){ //Parse Variable Declaration
            return parseVariableDeclaration(tokenList, index);
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
            std::cout << "velc: Parser: Invalid token in parseReturn: Expected Semicolon\n";
            exit(1);
        }
        advance(tokenList, index, 1); //skip SEMICOLON

        return returnNode;
    }

    std::unique_ptr<If> parseIf(const std::vector<Token>& tokenList, int& index){
        std::unique_ptr<If> ifNode = std::make_unique<If>(If{});
        advance(tokenList, index, 1); //skip if keyword

        //skip LPAREN
        if(!isType(getToken(tokenList, index), TokenType::LPAREN)){ //Check if the token is LPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseIf: Expected Left Parenthesis when parsing if\n";
            exit(1);
        }
        advance(tokenList, index, 1); //skip LPAREN

        //parse the condition
        ifNode->Condition = parseExpression(tokenList, index);
        
        //skip RPAREN
        if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseIf: Expected Right Parenthesis when parsing if\n";
            exit(1);
        }
        advance(tokenList, index, 1); //skip RPAREN

        //parse the body
        ifNode->Body = parseStatement(tokenList, index);

        //parse elseifs
        while(isType(getToken(tokenList, index), TokenType::ELSE_KEYWORD) && isType(getToken(tokenList, index, 1), TokenType::IF_KEYWORD)){
            ElseIf elseIfNode;
            //skip the else and if keywords
            advance(tokenList, index, 2);

            //skip LPAREN
            if(!isType(getToken(tokenList, index), TokenType::LPAREN)){ //Check if the token is LPAREN
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::cout << "velc: Parser: Invalid token in parseIf: Expected Left Parenthesis when parsing elseif\n";
                exit(1);
            }
            advance(tokenList, index, 1); //skip LPAREN

            //parse the condition
            elseIfNode.Condition = parseExpression(tokenList, index);
            
            //skip RPAREN
            if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::cout << "velc: Parser: Invalid token in parseIf: Expected Right Parenthesis when parsing elseif\n";
                exit(1);
            }
            advance(tokenList, index, 1); //skip RPAREN

            //parse the body
            elseIfNode.Body = parseStatement(tokenList, index);

            //add else if to the list
            ifNode->ElseIfs.push_back(std::move(elseIfNode));
        }

        //parse else
        if(isType(getToken(tokenList, index), TokenType::ELSE_KEYWORD)){
            std::unique_ptr<Statement> elseNode = std::make_unique<Statement>(Statement{});
            advance(tokenList, index, 1); //skip else keyword

            //parse the body
            elseNode = parseStatement(tokenList, index);

            //add to If
            ifNode->Else = std::move(elseNode);
        }

        return ifNode;
    }

    std::unique_ptr<While> parseWhile(const std::vector<Token>& tokenList, int& index){
        std::unique_ptr<While> whileNode = std::make_unique<While>(While{});
        advance(tokenList, index, 1); //skip while keyword

        //skip LPAREN
        if(!isType(getToken(tokenList, index), TokenType::LPAREN)){ //Check if the token is LPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseWhile: Expected Left Parenthesis\n";
            exit(1);
        }
        advance(tokenList, index, 1); //skip LPAREN

        //parse the condition
        whileNode->Condition = parseExpression(tokenList, index);
        
        //skip RPAREN
        if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseWhile: Expected Right Parenthesis\n";
            exit(1);
        }
        advance(tokenList, index, 1); //skip RPAREN

        //parse the body
        whileNode->Body = parseStatement(tokenList, index);

        return whileNode;
    }

    std::unique_ptr<For> parseFor(const std::vector<Token>& tokenList, int& index){
        std::unique_ptr<For> forNode = std::make_unique<For>(For{});
        advance(tokenList, index, 1); //skip for keyword
        //skip LPAREN
        if(!isType(getToken(tokenList, index), TokenType::LPAREN)){ //Check if the token is LPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseFor: Expected Left Parenthesis\n";
            exit(1);
        }
        advance(tokenList, index, 1); //skip LPAREN

        //parse the insides
        if(isTypename(getToken(tokenList, index)) && isType(getToken(tokenList, index, 1), TokenType::IDENTIFIER)){ //Variable Declaration
            forNode->Initializer = parseVariableDeclaration(tokenList, index, TokenType::COMMA); //auto consumes comma
        }
        else{
            forNode->Initializer = parseExpression(tokenList, index); //Expression
            //skip COMMA
            if(!isType(getToken(tokenList, index), TokenType::COMMA)){ //Check if the token is COMMA
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::cout << "velc: Parser: Invalid token in parseFor: Expected Comma after initializer\n";
                exit(1);
            }
            advance(tokenList, index, 1); //skip COMMA
        }
        //condition
        forNode->Condition = parseExpression(tokenList, index); //Expression
        //skip COMMA
        if(!isType(getToken(tokenList, index), TokenType::COMMA)){ //Check if the token is COMMA
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseFor: Expected Comma after condition\n";
            exit(1);
        }
        advance(tokenList, index, 1); //skip COMMA

        //modifier
        forNode->Modifier = parseExpression(tokenList, index); //Expression
        
        //skip RPAREN
        if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "velc: Parser: Invalid token in parseWhile: Expected Right Parenthesis\n";
            exit(1);
        }
        advance(tokenList, index, 1); //skip RPAREN

        //parse the body
        forNode->Body = parseStatement(tokenList, index);

        return forNode;
    }

    std::unique_ptr<ExpressionStatement> parseExpressionStatement(const std::vector<Token>& tokenList, int& index, const TokenType& stopType){
        std::unique_ptr<ExpressionStatement> expressionStatementNode = std::make_unique<ExpressionStatement>(ExpressionStatement{});
        expressionStatementNode->Expr = parseExpression(tokenList, index);

        //skip stop token
        if(!isType(getToken(tokenList, index), stopType)){ //Check if the token is stop token
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            std::cout << "Invalid token in parseExpressionStatement: Expected ";
            std::cout << ((stopType == TokenType::SEMICOLON) ? "Semicolon" : "Stop token");
            exit(1);
        }
        advance(tokenList, index, 1); //skip stop token

        return expressionStatementNode;
    }

    int getPrecedence(const TokenType& type, const uint8_t& mode){ // 0 for binary, 1 for prefix, 2 for postfix
        if(mode == 1){
            if(binding_power_table_prefix.find(type) != binding_power_table_prefix.end()){ //found
                return binding_power_table_prefix.at(type) * 1000000;
            }
        }
        else if(mode == 2){
            if(binding_power_table_postfix.find(type) != binding_power_table_postfix.end()){ //found
                return binding_power_table_postfix.at(type) * 1000000;
            }
        }
        else{
            if(binding_power_table.find(type) != binding_power_table.end()){ //found
                return binding_power_table.at(type) * 1000000;
            }
        }
        return -1;
    }

    std::unique_ptr<Expression> parseExpression(const std::vector<Token>& tokenList, int& index, int minPrecedence){
        Token current = getToken(tokenList, index);
        if(current.type == TokenType::SEMICOLON || current.type == TokenType::RPAREN || current.type == TokenType::COMMA){
            return nullptr;
        }
        std::unique_ptr<Expression> left = parsePrefixExpression(tokenList, index);

        left = parsePostfixExpression(tokenList, index, std::move(left));
        //binary loop
        while(true){
            Token currentToken = getToken(tokenList, index);
            int precedence = getPrecedence(currentToken.type);
            if(precedence < minPrecedence){
                break;
            }
            bool rightAssoc = (
                currentToken.type == TokenType::ASSIGN ||
                currentToken.type == TokenType::ADD_ASSIGN ||
                currentToken.type == TokenType::SUB_ASSIGN ||
                currentToken.type == TokenType::MUL_ASSIGN ||
                currentToken.type == TokenType::DIV_ASSIGN ||
                currentToken.type == TokenType::MOD_ASSIGN ||
                currentToken.type == TokenType::AND_ASSIGN ||
                currentToken.type == TokenType::OR_ASSIGN ||
                currentToken.type == TokenType::XOR_ASSIGN ||
                currentToken.type == TokenType::LSHIFT_ASSIGN ||
                currentToken.type == TokenType::RSHIFT_ASSIGN ||
                currentToken.type == TokenType::NOT_ASSIGN
            );
            int newMinPrec = rightAssoc ? precedence : precedence+1;

            advance(tokenList, index, 1); //skip the operator
            std::unique_ptr<Expression> right = parseExpression(tokenList, index, newMinPrec);

            BinaryOperator newBinaryOp;
            bool isAssignment = false;
            //convert to binary operator
            switch(currentToken.type){
                case TokenType::ADD:
                newBinaryOp = BinaryOperator::ADD;
                break;
                case TokenType::SUB:
                newBinaryOp = BinaryOperator::SUB;
                break;
                case TokenType::MUL:
                newBinaryOp = BinaryOperator::MUL;
                break;
                case TokenType::DIV:
                newBinaryOp = BinaryOperator::DIV;
                break;
                case TokenType::MOD:
                newBinaryOp = BinaryOperator::MOD;
                break;

                case TokenType::EQ:
                newBinaryOp = BinaryOperator::EQ;
                break;
                case TokenType::NEQ:
                newBinaryOp = BinaryOperator::NEQ;
                break;
                case TokenType::LT:
                newBinaryOp = BinaryOperator::LT;
                break;
                case TokenType::GT:
                newBinaryOp = BinaryOperator::GT;
                break;
                case TokenType::LTE:
                newBinaryOp = BinaryOperator::LTE;
                break;
                case TokenType::GTE:
                newBinaryOp = BinaryOperator::GTE;
                break;

                case TokenType::AND:
                newBinaryOp = BinaryOperator::BITWISE_AND;
                break;
                case TokenType::OR:
                newBinaryOp = BinaryOperator::BITWISE_OR;
                break;
                case TokenType::XOR:
                newBinaryOp = BinaryOperator::BITWISE_XOR;
                break;
                case TokenType::LSHIFT:
                newBinaryOp = BinaryOperator::LSHIFT;
                break;
                case TokenType::RSHIFT:
                newBinaryOp = BinaryOperator::RSHIFT;
                break;

                case TokenType::ANDAND:
                newBinaryOp = BinaryOperator::LOGICAL_AND;
                break;
                case TokenType::OROR:
                newBinaryOp = BinaryOperator::LOGICAL_OR;
                break;
                case TokenType::XORXOR:
                newBinaryOp = BinaryOperator::LOGICAL_XOR;
                break;

                case TokenType::ASSIGN:
                newBinaryOp = BinaryOperator::ASSIGN;isAssignment = true;
                break;
                case TokenType::ADD_ASSIGN:
                newBinaryOp = BinaryOperator::ADD_ASSIGN;isAssignment = true;
                break;
                case TokenType::SUB_ASSIGN:
                newBinaryOp = BinaryOperator::SUB_ASSIGN;isAssignment = true;
                break;
                case TokenType::MUL_ASSIGN:
                newBinaryOp = BinaryOperator::MUL_ASSIGN;isAssignment = true;
                break;
                case TokenType::DIV_ASSIGN:
                newBinaryOp = BinaryOperator::DIV_ASSIGN;isAssignment = true;
                break;
                case TokenType::MOD_ASSIGN:
                newBinaryOp = BinaryOperator::MOD_ASSIGN;isAssignment = true;
                break;
                case TokenType::AND_ASSIGN:
                newBinaryOp = BinaryOperator::AND_ASSIGN;isAssignment = true;
                break;
                case TokenType::OR_ASSIGN:
                newBinaryOp = BinaryOperator::OR_ASSIGN;isAssignment = true;
                break;
                case TokenType::XOR_ASSIGN:
                newBinaryOp = BinaryOperator::XOR_ASSIGN;isAssignment = true;
                break;
                case TokenType::LSHIFT_ASSIGN:
                newBinaryOp = BinaryOperator::LSHIFT_ASSIGN;isAssignment = true;
                break;
                case TokenType::RSHIFT_ASSIGN:
                newBinaryOp = BinaryOperator::RSHIFT_ASSIGN;isAssignment = true;
                break;
                case TokenType::NOT_ASSIGN:
                newBinaryOp = BinaryOperator::NOT_ASSIGN;isAssignment = true;
                break;
                case TokenType::SWAP:
                newBinaryOp = BinaryOperator::SWAP;isAssignment = true;
                break;

                default:
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::cout << "velc: Parser: Invalid token type in parseExpression: Expected Binary Operator type when converting to a binary operator\n";
                exit(1);
            };
            if(!isAssignment){ //make left a binary node
                std::unique_ptr<BinaryOp> binaryNode = std::make_unique<BinaryOp>(BinaryOp{});
                binaryNode->Op = newBinaryOp;
                binaryNode->Left = std::move(left);
                binaryNode->Right = std::move(right);
                left = std::move(binaryNode);
            }
            else{ //assignment operator
                std::unique_ptr<AssignmentOp> assignmentNode = std::make_unique<AssignmentOp>(AssignmentOp{});
                assignmentNode->Op = newBinaryOp;
                assignmentNode->Left = std::move(left);
                assignmentNode->Right = std::move(right);
                left = std::move(assignmentNode);
            }
        };
        return left;
    }

    std::unique_ptr<Expression> parsePrefixExpression(const std::vector<Token>& tokenList, int& index){
        Token currentToken = getToken(tokenList, index);

        if(isLiteral(currentToken)){
            advance(tokenList, index, 1);
            std::unique_ptr<Literal> literalNode = std::make_unique<Literal>(Literal{});
            switch(currentToken.type){
                case TokenType::INT_LITERAL:
                literalNode->Type = LiteralType::INT;
                literalNode->IntValue = currentToken.value;
                break;
                case TokenType::FLOAT_LITERAL:
                literalNode->Type = LiteralType::FLOAT;
                literalNode->FloatValue = currentToken.valuef;
                break;
                case TokenType::CHAR_LITERAL:
                literalNode->Type = LiteralType::CHAR;
                literalNode->CharValue = (char)currentToken.value;
                break;
                case TokenType::STRING_LITERAL:
                literalNode->Type = LiteralType::STRING;
                literalNode->StringValue = currentToken.text;
                break;
                default:
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::cout << "velc: Parser: Invalid token type in parsePrefixExpression: Expected Literal type when constructing literalNode\n";
                exit(1);
            }
            return literalNode;
        }
        if(isType(currentToken, TokenType::IDENTIFIER)){
            advance(tokenList, index, 1);
            std::unique_ptr<Identifier> identifierNode = std::make_unique<Identifier>(Identifier{});
            identifierNode->Text = currentToken.text;
            return identifierNode;
        }
        if(isType(currentToken, TokenType::LPAREN)){ //grouping, (
            advance(tokenList, index, 1); //skip LPAREN

            std::unique_ptr<Expression> expressionNode = parseExpression(tokenList, index);

            //skip RPAREN
            if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::cout << "velc: Parser: Invalid token in parsePrefixExpression: Expected Right Parenthesis\n";
                exit(1);
            }
            advance(tokenList, index, 1); //skip RPAREN

            return expressionNode;
        }

        if(getPrecedence(currentToken.type, 1) != -1){ //if it is a prefix operator
            advance(tokenList, index, 1); //skip the operator
            int precedence = getPrecedence(currentToken.type, true);
            std::unique_ptr<PrefixOp> prefixNode = std::make_unique<PrefixOp>(PrefixOp{});

            //convert to prefix operator
            switch(currentToken.type){
                case TokenType::NOT_LOGICAL:
                prefixNode->Op = PrefixOperator::LOGICAL_NOT;
                break;
                case TokenType::NOT_BITWISE:
                prefixNode->Op = PrefixOperator::BITWISE_NOT;
                break;
                case TokenType::INC:
                prefixNode->Op = PrefixOperator::INC;
                break;
                case TokenType::DEC:
                prefixNode->Op = PrefixOperator::DEC;
                break;
                case TokenType::ADD:
                prefixNode->Op = PrefixOperator::ADD;
                break;
                case TokenType::SUB:
                prefixNode->Op = PrefixOperator::NEG;
                break;
                default:
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::cout << "velc: Parser: Invalid token type in parsePrefixExpression: Expected Prefix Operator type when converting to a prefix operator\n";
                exit(1);
            };

            prefixNode->Expr = parseExpression(tokenList, index, precedence);
            return prefixNode;
        }

        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
        std::cout << "velc: Parser: Unexpected token in parsePrefixExpression: no valid token types recognized\n";
        exit(1);
    }

    std::unique_ptr<Expression> parsePostfixExpression(const std::vector<Token>& tokenList, int& index, std::unique_ptr<Expression> left){
        while(true){
            Token currentToken = getToken(tokenList, index);

            if(isType(currentToken, TokenType::LPAREN)){ //function call
                Identifier* id = dynamic_cast<Identifier*>(left.get());
                if(!id){
                    break; //left is not an identifier
                }

                std::unique_ptr<Call> callNode = std::make_unique<Call>(Call{});
                callNode->Id = std::unique_ptr<Identifier>(static_cast<Identifier*>(left.release()));

                advance(tokenList, index, 1); //skip LPAREN
                //call arguments
                while(!isType(getToken(tokenList, index), TokenType::RPAREN)){
                    std::unique_ptr<Expression> argument = parseExpression(tokenList, index);
                    callNode->Arguments.push_back(std::move(argument));
                    //continue if COMMA
                    if(isType(getToken(tokenList, index), TokenType::COMMA)){ //Check if the token is COMMA
                        advance(tokenList, index, 1); //skip COMMA
                        continue;
                    }
                    break;
                }
                //skip RPAREN
                if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
                    std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                    std::cout << "velc: Parser: Invalid token in parsePostfixExpression: Expected Right Parenthesis when parsing function call arguments\n";
                    exit(1);
                }
                advance(tokenList, index, 1); //skip RPAREN

                left = std::move(callNode);
                continue;
            }
            
            if(getPrecedence(currentToken.type, 2) != -1){ //if is postfix operator
                advance(tokenList, index, 1); //skip current postfix operator
                std::unique_ptr<PostfixOp> postfixNode = std::make_unique<PostfixOp>(PostfixOp{});
                postfixNode->Expr = std::move(left);


                //convert to postfix operator
                switch(currentToken.type){
                    case TokenType::INC:
                    postfixNode->Op = PostfixOperator::INC;
                    break;
                    case TokenType::DEC:
                    postfixNode->Op = PostfixOperator::DEC;
                    break;
                    default:
                    std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                    std::cout << "velc: Parser: Invalid token type in parsePostfixExpression: Expected Postfix Operator type when converting to a postfix operator\n";
                    exit(1);
                };


                left = std::move(postfixNode);
                continue;
            }
            break;
        }
        return left;
    }
}



