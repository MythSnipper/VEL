#include <parser.hpp>

Program constructAST(std::vector<Token> tokenList){
    Program program;
    int index = 0;

    //first construct program.TopLevel
    while(!isType(getToken(tokenList, index), TokenType::END_OF_FILE)){ //while current token is not EOF
        Token currentToken = getToken(tokenList, index);
        Token nextToken = getToken(tokenList, index, 1);
        Token nextnextToken = getToken(tokenList, index, 2);

        if(isType(currentToken, TokenType::ASSEMBLY)){
            std::unique_ptr<Assembly> AssemblyNode = std::make_unique<Assembly>(Assembly{});
            AssemblyNode->Text = currentToken.text;

            std::cout << "\tAssembly created\nText: " << AssemblyNode->Text << "\n";

            program.TopLevel.push_back(std::move(AssemblyNode));
            advance(tokenList, index, 1);
        }
        else if(isTypename(currentToken) && isType(nextToken, TokenType::IDENTIFIER) && isType(nextnextToken, TokenType::LPAREN)){ //Function
            std::unique_ptr<Function> FunctionNode = std::make_unique<Function>(Function{});
            FunctionNode->ReturnType = Type{true, convertType(currentToken.type), ""};

            FunctionNode->Id = std::make_unique<Identifier>(Identifier{});
            FunctionNode->Id->Text = nextToken.text;
            
            FunctionNode->Body = std::make_unique<Block>(Block{});

            std::cout << "\tFunction created\nReturnType: " << (int)FunctionNode->ReturnType.builtinType << "\nId: " << FunctionNode->Id->Text << "\n";

            program.TopLevel.push_back(std::move(FunctionNode));
            advance(tokenList, index, 1);
        }















    }


    return program;
};

//get a token with offset
Token getToken(const std::vector<Token>& tokenList, const int& index, const int& offset){
    return (index < tokenList.size()-offset) ? tokenList[index+offset] : Token{TokenType::INVALID_TOKEN};
}

//advance
void advance(const std::vector<Token>& tokenList, int& index, const int& numTokens){
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





