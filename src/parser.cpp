#include <parser.hpp>

std::unordered_map<TokenType, int> binding_power_table = {

    {TokenType::MUL, 12000000},                 // *
    {TokenType::DIV, 12000000},                 // /
    {TokenType::MOD, 12000000},                 // %

    {TokenType::ADD, 11000000},                 // +
    {TokenType::SUB, 11000000},                 // -

    {TokenType::LSHIFT, 10000000},              // <<
    {TokenType::RSHIFT, 10000000},              // >>

    {TokenType::LT, 9000000},                  // <
    {TokenType::GT, 9000000},                  // >
    {TokenType::LTE, 9000000},                 // <=
    {TokenType::GTE, 9000000},                 // >=

    {TokenType::EQ, 8000000},                   // ==
    {TokenType::NEQ, 8000000},                  // != 

    {TokenType::AND, 7000000},                  // &
    {TokenType::XOR, 6000000},                  // ^
    {TokenType::OR, 5000000},                   // |
    
    {TokenType::ANDAND, 4000000},               // &&
    {TokenType::OROR, 3000000},                 // ||
    {TokenType::SWAP, 2000000},                 // ^^

    {TokenType::ASSIGN, 1000000},               // =
    {TokenType::ADD_ASSIGN, 1000000},           // +=
    {TokenType::SUB_ASSIGN, 1000000},           // -=
    {TokenType::MUL_ASSIGN, 1000000},           // *=
    {TokenType::DIV_ASSIGN, 1000000},           // /=
    {TokenType::MOD_ASSIGN, 1000000},           // %=
    {TokenType::AND_ASSIGN, 1000000},           // &=
    {TokenType::OR_ASSIGN, 1000000},            // |=
    {TokenType::XOR_ASSIGN, 1000000},           // ^=
    {TokenType::LSHIFT_ASSIGN, 1000000},        // <<=
    {TokenType::RSHIFT_ASSIGN, 1000000},        // >>=
    {TokenType::NOT_ASSIGN, 1000000},           // !~=
    
};
std::unordered_map<TokenType, int> binding_power_table_postfix = {
    //POSTFIX
    {TokenType::INC, 13000000},                 // ++
    {TokenType::DEC, 13000000},                 // -- 
};
std::unordered_map<TokenType, int> binding_power_table_prefix = {
    //PREFIX
    {TokenType::NOT_LOGICAL, 14000000},         // !
    {TokenType::NOT_BITWISE, 14000000},         // !~
    {TokenType::INC, 14000000},                 // ++
    {TokenType::DEC, 14000000},                 // --
    {TokenType::ADD, 14000000},                 // +
    {TokenType::SUB, 14000000},                 // -

};

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
        FunctionNode->Body = parseStatement(tokenList, index);

        std::cout << "\tFunction parsed\nReturnType: " << (int)FunctionNode->ReturnType.builtinType << "\nId: " << FunctionNode->Id->Text << "\n";

        return FunctionNode;
    }

    std::unique_ptr<GlobalVariableDeclaration> parseGlobalVariableDeclaration(const std::vector<Token>& tokenList, int& index){
        std::unique_ptr<GlobalVariableDeclaration> GlobalVariableDeclarationNode = std::make_unique<GlobalVariableDeclaration>(GlobalVariableDeclaration{});
        GlobalVariableDeclarationNode->Typename = Type{true, convertType(getToken(tokenList, index).type), ""}; //First token is the return type
        
        GlobalVariableDeclarationNode->Id = std::make_unique<Identifier>(Identifier{getToken(tokenList, index, 1).text}); //Second token is the function identifier
        
        advance(tokenList, index, 2); //skip typename, identifier

        if(isType(getToken(tokenList, index), TokenType::ASSIGN)){
            advance(tokenList, index, 1); //skip assignment operator
            GlobalVariableDeclarationNode->Expr = parseExpression(tokenList, index); //parse expression till ;
            //skip Semicolon
            if(!isType(getToken(tokenList, index), TokenType::SEMICOLON)){ //Check if the token is SEMICOLON
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                throw std::runtime_error("Invalid token in parseGlobalVariableDeclaration: Expected Semicolon");
            }
            advance(tokenList, index, 1); //skip SEMICOLON
        }
        else if(isType(getToken(tokenList, index), TokenType::SEMICOLON)){
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

    std::unique_ptr<VariableDeclaration> parseVariableDeclaration(const std::vector<Token>& tokenList, int& index, const TokenType& stopType){
        std::unique_ptr<VariableDeclaration> VariableDeclarationNode = std::make_unique<VariableDeclaration>(VariableDeclaration{});
        VariableDeclarationNode->Typename = Type{true, convertType(getToken(tokenList, index).type), ""}; //First token is the return type
        
        VariableDeclarationNode->Id = std::make_unique<Identifier>(Identifier{getToken(tokenList, index, 1).text}); //Second token is the function identifier
        
        advance(tokenList, index, 2); //skip typename, identifier

        if(isType(getToken(tokenList, index), TokenType::ASSIGN)){
            advance(tokenList, index, 1); //skip assignment operator
            VariableDeclarationNode->Expr = parseExpression(tokenList, index); //parse expression till ;
            //skip stop token
            if(!isType(getToken(tokenList, index), stopType)){ //Check if the token is stop token
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                std::string errorstr = "Invalid token in parseVariableDeclaration: Expected ";
                errorstr += ((stopType == TokenType::SEMICOLON) ? "Semicolon" : "Stop token");
                throw std::runtime_error(errorstr.c_str());
            }
            advance(tokenList, index, 1); //skip stop token
        }
        else if(isType(getToken(tokenList, index), TokenType::SEMICOLON)){
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
            throw std::runtime_error("Invalid token in parseReturn: Expected Semicolon");
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
            throw std::runtime_error("Invalid token in parseIf: Expected Left Parenthesis when parsing if");
        }
        advance(tokenList, index, 1); //skip LPAREN

        //parse the condition
        ifNode->Condition = parseExpression(tokenList, index);
        
        //skip RPAREN
        if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            throw std::runtime_error("Invalid token in parseIf: Expected Right Parenthesis when parsing if");
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
                throw std::runtime_error("Invalid token in parseIf: Expected Left Parenthesis when parsing elseif");
            }
            advance(tokenList, index, 1); //skip LPAREN

            //parse the condition
            elseIfNode.Condition = parseExpression(tokenList, index);
            
            //skip RPAREN
            if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                throw std::runtime_error("Invalid token in parseIf: Expected Right Parenthesis when parsing elseif");
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
            throw std::runtime_error("Invalid token in parseWhile: Expected Left Parenthesis");
        }
        advance(tokenList, index, 1); //skip LPAREN

        //parse the condition
        whileNode->Condition = parseExpression(tokenList, index);
        
        //skip RPAREN
        if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            throw std::runtime_error("Invalid token in parseWhile: Expected Right Parenthesis");
        }
        advance(tokenList, index, 1); //skip RPAREN

        //parse the body
        whileNode->Body = parseStatement(tokenList, index);

        return whileNode;
    }

    std::unique_ptr<For> parseFor(const std::vector<Token>& tokenList, int& index){
        std::unique_ptr<For> forNode = std::make_unique<For>(For{});
        //skip LPAREN
        if(!isType(getToken(tokenList, index), TokenType::LPAREN)){ //Check if the token is LPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            throw std::runtime_error("Invalid token in parseFor: Expected Left Parenthesis");
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
                throw std::runtime_error("Invalid token in parseFor: Expected Comma after initializer");
            }
            advance(tokenList, index, 1); //skip COMMA
        }
        //condition
        forNode->Initializer = parseExpression(tokenList, index); //Expression
        //skip COMMA
        if(!isType(getToken(tokenList, index), TokenType::COMMA)){ //Check if the token is COMMA
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            throw std::runtime_error("Invalid token in parseFor: Expected Comma after condition");
        }
        advance(tokenList, index, 1); //skip COMMA

        //modifier
        forNode->Initializer = parseExpression(tokenList, index); //Expression
        
        //skip RPAREN
        if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
            std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
            throw std::runtime_error("Invalid token in parseWhile: Expected Right Parenthesis");
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
            std::string errorstr = "Invalid token in parseExpressionStatement: Expected ";
            errorstr += ((stopType == TokenType::SEMICOLON) ? "Semicolon" : "Stop token");
            throw std::runtime_error(errorstr.c_str());
        }
        advance(tokenList, index, 1); //skip stop token

        return expressionStatementNode;
    }

    int getPrecedence(const TokenType& type, const uint8_t& mode){ // 0 for binary, 1 for prefix, 2 for postfix
        if(mode == 1){
            if(binding_power_table_prefix.find(type) != binding_power_table_prefix.end()){ //found
                return binding_power_table_prefix.at(type);
            }
        }
        else if(mode == 2){
            if(binding_power_table_postfix.find(type) != binding_power_table_postfix.end()){ //found
                return binding_power_table_postfix.at(type);
            }
        }
        else{
            if(binding_power_table.find(type) != binding_power_table.end()){ //found
                return binding_power_table.at(type);
            }
        }
        return -1;
    }

    std::unique_ptr<Expression> parseExpression(const std::vector<Token>& tokenList, int& index, uint8_t minPrecedence){
        std::unique_ptr<Expression> left = parsePrefixExpression(tokenList, index);

        left = parsePostfixExpression(tokenList, index, std::move(left));
        while(true){
            Token currentToken = getToken(tokenList, index);
            int precedence = getPrecedence(currentToken.type);
            if(precedence < minPrecedence){
                break;
            }
            int leftPrecedence = precedence;
            int rightPrecedence = leftPrecedence + 1;
            
            advance(tokenList, index, 1);
            std::unique_ptr<Expression> right = parseExpression(tokenList, index, rightPrecedence);

            //make left a binary node
            std::unique_ptr<BinaryOp> binaryNode = std::make_unique<BinaryOp>(BinaryOp{});
            binaryNode->Left = std::move(left);
            binaryNode->Op = toBinaryOperator(currentToken.type);
            binaryNode->Right = std::move(right);

            left = std::move(binaryNode);
        };
        return left;
    }

    std::unique_ptr<Expression> parsePrefixExpression(const std::vector<Token>& tokenList, int& index){
        Token currentToken = getToken(tokenList, index);

        if(isLiteral(currentToken)){
            advance(tokenList, index, 1);
            return makeLiteralExpression(currentToken);
        }
        if(isType(currentToken, TokenType::IDENTIFIER)){
            advance(tokenList, index, 1);
            return makeIdentifierExpression(currentToken);
        }
        if(isType(currentToken, TokenType::LPAREN)){ //grouping, (
            advance(tokenList, index, 1); //skip LPAREN

            std::unique_ptr<Expression> expressionNode = parseExpression(tokenList, index);

            //skip RPAREN
            if(!isType(getToken(tokenList, index), TokenType::RPAREN)){ //Check if the token is RPAREN
                std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
                throw std::runtime_error("Invalid token in parsePrefixExpression: Expected Right Parenthesis");
            }
            advance(tokenList, index, 1); //skip RPAREN

            return expressionNode;
        }

        if(getPrecedence(currentToken.type, 1) != -1){ //if it is a prefix operator
            advance(tokenList, index, 1);
            uint8_t precedence = getPrecedence(currentToken.type, true);
            std::unique_ptr<PrefixOp> prefixNode = std::make_unique<PrefixOp>(PrefixOp{});
            prefixNode->Op = toPrefixOperator(currentToken.type);
            prefixNode->Expr = parseExpression(tokenList, index, precedence);
            return prefixNode;
        }



        std::cout << "Info:\nCurrent token: " << getToken(tokenList, index).text << "Next token: " << getToken(tokenList, index, 1).text << "\n";
        throw std::runtime_error("Unexpected token in parsePrefixExpression: no valid token types recognized");
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
                    throw std::runtime_error("Invalid token in parsePostfixExpression: Expected Right Parenthesis when parsing function call arguments");
                }
                advance(tokenList, index, 1); //skip RPAREN

                left = std::move(callNode);
                continue;
            }
            
            if(getPrecedence(currentToken.type, 2) != -1){ //if is postfix operator
                std::unique_ptr<PostfixOp> postfixNode = std::make_unique<PostfixOp>(PostfixOp{});
                postfixNode->Expr = std::move(left);
                postfixNode->Op = toPostfixOperator(currentToken.type);
                advance(tokenList, index, 1); //skip current postfix operator
                left = std::move(postfixNode);
                continue;
            }
            break;
        }
        return left;
    }

    BinaryOperator toBinaryOperator(const TokenType& type){
        switch(type){
            
        };
    }

};
