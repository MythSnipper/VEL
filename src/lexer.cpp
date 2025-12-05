#include <lexer.hpp>


namespace Lexer{

    std::unordered_map<std::string, TokenType> map_kwid = {
        {"vel", TokenType::VEL_KEYWORD},
        {"int8", TokenType::INT8_KEYWORD},
        {"int16", TokenType::INT16_KEYWORD},
        {"int32", TokenType::INT32_KEYWORD},
        {"int64", TokenType::INT64_KEYWORD},
        {"uint8", TokenType::UINT8_KEYWORD},
        {"uint16", TokenType::UINT16_KEYWORD},
        {"uint32", TokenType::UINT32_KEYWORD},
        {"uint64", TokenType::UINT64_KEYWORD},
        {"char", TokenType::CHAR_KEYWORD},
        {"float32", TokenType::FLOAT32_KEYWORD},
        {"float64", TokenType::FLOAT64_KEYWORD},
        {"string", TokenType::STRING_KEYWORD},
        {"void", TokenType::VOID_KEYWORD},
        {"if", TokenType::IF_KEYWORD},
        {"else", TokenType::ELSE_KEYWORD},
        {"while", TokenType::WHILE_KEYWORD},
        {"for", TokenType::FOR_KEYWORD},
        {"return", TokenType::RETURN_KEYWORD}
    };
    std::unordered_map<TokenType, std::string> map_idkw = {
        {TokenType::VEL_KEYWORD, "VEL :3"},
        {TokenType::INT8_KEYWORD, "Keyword int8"},
        {TokenType::INT16_KEYWORD, "Keyword int16"},
        {TokenType::INT32_KEYWORD, "Keyword int32"},
        {TokenType::INT64_KEYWORD, "Keyword int64"},
        {TokenType::UINT8_KEYWORD, "Keyword uint8"},
        {TokenType::UINT16_KEYWORD, "Keyword uint16"},
        {TokenType::UINT32_KEYWORD, "Keyword uint32"},
        {TokenType::UINT64_KEYWORD, "Keyword uint64"},
        {TokenType::CHAR_KEYWORD, "Keyword char"},
        {TokenType::FLOAT32_KEYWORD, "Keyword float32"},
        {TokenType::FLOAT64_KEYWORD, "Keyword float64"},
        {TokenType::STRING_KEYWORD, "Keyword string"},
        {TokenType::VOID_KEYWORD, "Keyword void"},
        {TokenType::IF_KEYWORD, "Keyword if"},
        {TokenType::ELSE_KEYWORD, "Keyword else"},
        {TokenType::WHILE_KEYWORD, "Keyword while"},
        {TokenType::FOR_KEYWORD, "Keyword for"},
        {TokenType::RETURN_KEYWORD, "Keyword return"},
        {TokenType::IDENTIFIER, "Identifier"},
        {TokenType::INT_LITERAL, "Literal Int"},
        {TokenType::FLOAT_LITERAL, "Literal Float"},
        {TokenType::CHAR_LITERAL, "Literal Char"},
        {TokenType::STRING_LITERAL, "Literal String"},
        {TokenType::ASSEMBLY, "Assembly :3"},
        {TokenType::SEMICOLON, "Semicolon"}, 
        {TokenType::COMMA, "Comma"},
        {TokenType::LPAREN, "LParen"},
        {TokenType::RPAREN, "RParen"},
        {TokenType::LBRACE, "LBrace"},
        {TokenType::RBRACE, "RBrace"},
        {TokenType::ASSIGN, "Assign"},
        {TokenType::ADD_ASSIGN, "ADD Assign"},
        {TokenType::SUB_ASSIGN, "SUB Assign"},
        {TokenType::MUL_ASSIGN, "MUL Assign"},
        {TokenType::DIV_ASSIGN, "DIV Assign"},
        {TokenType::MOD_ASSIGN, "MOD Assign"},
        {TokenType::AND_ASSIGN, "AND Assign"},
        {TokenType::OR_ASSIGN, "OR Assign"},
        {TokenType::XOR_ASSIGN, "XOR Assign"},
        {TokenType::LSHIFT_ASSIGN, "LSHIFT Assign"},
        {TokenType::RSHIFT_ASSIGN, "RSHIFT Assign"},
        {TokenType::NOT_ASSIGN, "NOT Assign"},
        {TokenType::SWAP, "SWAP"},
        {TokenType::ADD, "ADD"},
        {TokenType::SUB, "SUB"},
        {TokenType::MUL, "MUL"},
        {TokenType::DIV, "DIV"},
        {TokenType::MOD, "MOD"},
        {TokenType::EQ, "EQ"},
        {TokenType::NEQ, "NEQ"},
        {TokenType::LT, "LT"},
        {TokenType::GT, "GT"},
        {TokenType::LTE, "LTE"},
        {TokenType::GTE, "GTE"},
        {TokenType::AND, "AND"},
        {TokenType::ANDAND, "ANDAND"},
        {TokenType::OR, "OR"},
        {TokenType::OROR, "OROR"},
        {TokenType::XOR, "XOR"},    
        {TokenType::XORXOR, "XORXOR"},    
        {TokenType::LSHIFT, "LSHIFT"},
        {TokenType::RSHIFT, "RSHIFT"},
        {TokenType::NOT_BITWISE, "Bitwise NOT"},
        {TokenType::NOT_LOGICAL, "Logical NOT"},
        {TokenType::INC, "INC"},
        {TokenType::DEC, "DEC"},
        {TokenType::END_OF_FILE, "EOF"},
    };

    std::vector<Token> tokenize(const std::string& source){
        std::vector<Token> token_list;

        Token current_token;
        int line = 1;
        int col = 1;

        for(uint32_t i=0;i<source.length();){
            char current_char = source[i];
            char next_char = (i != source.length()-1) ? source[i+1] : '\0';
            char next_next_char = (i < source.length()-2) ? source[i+2] : '\0';
            
            
            //comments
            if(current_char == '/' && next_char == '/'){ //single line comment
                scanComment_Single(source, i, line, col);
            }
            else if(current_char == '/' && next_char == '{'){ //multi line comment
                scanComment_Multi(source, i, line, col);
            }



            //assembly
            else if(current_char == '%' && next_char == '%'){ //single line assembly
                current_token = scanAssembly_Single(source, i, line, col);
                token_list.push_back(current_token);
            }
            else if(current_char == '%' && next_char == '{'){ //multi line assembly
                current_token = scanAssembly_Multi(source, i, line, col);
                token_list.push_back(current_token);
            }

            //whitespace characters to skip
            else if(current_char == ' ' || current_char == '\n' || current_char == '\t'){
                advance(source, i, line, col);
            }



            //scan for keyword or identifier
            else if(isalpha(current_char) || current_char == '_'){
                current_token = scanKeywordOrIdentifier(source, i, line, col);
                token_list.push_back(current_token);
            }



            //scan for literals
            else if(isdigit(current_char)){
                current_token = scanNumLiteral(source, i, line, col);
                token_list.push_back(current_token);
            }
            else if(current_char == '\"'){
                current_token = scanStringLiteral(source, i, line, col);
                token_list.push_back(current_token);
            }
            else if(current_char == '\''){
                current_token = scanCharLiteral(source, i, line, col);
                token_list.push_back(current_token);
            }
            

            //grammar, punctuation stuff
            else if(current_char == ';'){
                current_token = (Token){TokenType::SEMICOLON, ";", 0, 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == ','){
                current_token = (Token){TokenType::COMMA, ",", 0, 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '('){
                current_token = (Token){TokenType::LPAREN, "(", 0, 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == ')'){
                current_token = (Token){TokenType::RPAREN, ")", 0, 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '{'){
                current_token = (Token){TokenType::LBRACE, "{", 0, 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '}'){
                current_token = (Token){TokenType::RBRACE, "}", 0, 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }




            //operators
            else if(current_char == '+'){
                if(next_char == '='){
                    current_token = (Token){TokenType::ADD_ASSIGN, "+=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '+'){
                    current_token = (Token){TokenType::INC, "++", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::ADD, "+", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '-'){
                if(next_char == '='){
                    current_token = (Token){TokenType::SUB_ASSIGN, "-=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '-'){
                    current_token = (Token){TokenType::DEC, "--", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::SUB, "-", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '*'){
                if(next_char == '='){
                    current_token = (Token){TokenType::MUL_ASSIGN, "*=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::MUL, "*", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '/'){
                if(next_char == '='){
                    current_token = (Token){TokenType::DIV_ASSIGN, "/=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::DIV, "/", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '%'){
                if(next_char == '='){
                    current_token = (Token){TokenType::MOD_ASSIGN, "%=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::MOD, "%", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '='){
                if(next_char == '='){
                    current_token = (Token){TokenType::EQ, "==", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::ASSIGN, "=", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '!'){
                if(next_char == '='){
                    current_token = (Token){TokenType::NEQ, "!=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '~'){
                    if(next_next_char == '='){
                        current_token = (Token){TokenType::NOT_ASSIGN, "!~=", 0, 0, line, col};
                        advance(source, i, line, col);
                    }
                    else{
                        current_token = (Token){TokenType::NOT_BITWISE, "!~", 0, 0, line, col};
                    }
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::NOT_LOGICAL, "!", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '<'){
                if(next_char == '='){
                    current_token = (Token){TokenType::LTE, "<=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '<'){
                    if(next_next_char == '='){
                        current_token = (Token){TokenType::LSHIFT_ASSIGN, "<<=", 0, 0, line, col};
                        advance(source, i, line, col);
                    }
                    else{
                        current_token = (Token){TokenType::LSHIFT, "<<", 0, 0, line, col};
                    }
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::LT, "<", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '>'){
                if(next_char == '='){
                    current_token = (Token){TokenType::GTE, ">=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '>'){
                    if(next_next_char == '='){
                        current_token = (Token){TokenType::RSHIFT_ASSIGN, ">>=", 0, 0, line, col};
                        advance(source, i, line, col);
                    }
                    else{
                        current_token = (Token){TokenType::RSHIFT, ">>", 0, 0, line, col};
                    }
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::GT, ">", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '&'){
                if(next_char == '&'){
                    current_token = (Token){TokenType::ANDAND, "&&", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '='){
                    current_token = (Token){TokenType::AND_ASSIGN, "&=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::AND, "&", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '|'){
                if(next_char == '|'){
                    current_token = (Token){TokenType::OROR, "||", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '='){
                    current_token = (Token){TokenType::OR_ASSIGN, "|=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::OR, "|", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '^'){
                if(next_char == '='){
                    current_token = (Token){TokenType::XOR_ASSIGN, "^=", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '^'){
                    current_token = (Token){TokenType::XORXOR, "^^", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::XOR, "^", 0, 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '$'){
                if(next_char == '$'){
                    current_token = (Token){TokenType::SWAP, "$$", 0, 0, line, col};
                    advance(source, i, line, col);
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }







            //invalid characters
            else{
                std::cout << "Invalid character: \'" << current_char << "\'\n";
                std::cout << "Next character: ";
                if(next_char != '\0'){
                    std::cout << '\'' << next_char << "\'\n";
                }
                else{
                    std::cout << "null\n";
                }
                std::cout << "velc: Lexer: Invalid character in source\n";
                exit(1);
                advance(source, i, line, col);
            }
        }

        token_list.push_back({TokenType::END_OF_FILE, "EOF!!!", -1, -1, line, col});
        return token_list;
    }
    void advance(const std::string& source, uint32_t& i, int& line, int& col){
        if(source[i] == '\n'){
            col = 1;
            line++;
        }
        else{
            col++;
        }
        i++;
    }
    Token scanKeywordOrIdentifier(const std::string& source, uint32_t& i, int& line, int& col){
        std::string current_text;

        int startline = line;
        int startcol = col;

        for(;i<source.length();){
            char current_char = source[i];
            if(isalnum(current_char) || current_char == '_'){
                current_text += current_char;
            }
            else{
                TokenType type = checkKeywordOrIdentifier(current_text);
                return {type, current_text, 0, 0, startline, startcol};
            }
            advance(source, i, line, col);
        }
        if(current_text.empty()) {
            // nothing was scanned, return INVALID
            return {TokenType::INVALID_TOKEN, "INVALID", 0, 0, startline, startcol};
        }
    
        TokenType type = checkKeywordOrIdentifier(current_text);
        return {type, current_text, 0, 0, startline, startcol};
    }
    TokenType checkKeywordOrIdentifier(const std::string& text){
        return (map_kwid.find(text) != map_kwid.end()) ? map_kwid[text] : TokenType::IDENTIFIER;

    }
    std::string KeywordToString(const TokenType& type){
        if((map_idkw.find(type) == map_idkw.end())){
            std::cout << "DEBUG\n" << (int32_t)type << "\n";
        }
        return (map_idkw.find(type) != map_idkw.end()) ? map_idkw[type] : "NUHUHTYPE";
    }
    
    
    Token scanNumLiteral(const std::string& source, uint32_t& i, int& line, int& col){
        std::string current_text;
        std::string current_text_no_underscore;

        int startline = line;
        int startcol = col;
        bool isFloat = false;

        for(;i<source.length();){
            char current_char = source[i];
            if(isdigit(current_char) || current_char == '_'){
                current_text += current_char;
                if(current_char != '_'){
                    current_text_no_underscore += current_char;
                }
            }
            else if(current_char == '.'){
                if(!isFloat){
                    current_text += current_char;
                    current_text_no_underscore += current_char;
                    isFloat = true;
                }
                else{ //return right away
                    TokenType type = TokenType::FLOAT_LITERAL;
                    double valuef;
                    try{
                        valuef = std::stod("0" + current_text_no_underscore);
                    }
                    catch(...){
                        std::cout << "Text: 0" << current_text_no_underscore;
                        std::cout << "velc: Lexer: Failed to convert string to float in scanNumLiteral\n";
                        exit(1);
                    }
                    return {type, current_text, 0, valuef, startline, startcol};
                }
            }
            else{
                if(isFloat){
                    TokenType type = TokenType::FLOAT_LITERAL;
                    double valuef;
                    try{
                        valuef = std::stod(current_text_no_underscore);
                    }
                    catch(const std::invalid_argument& e){
                        std::cout << "Text: " << current_text_no_underscore;
                        std::cout << "velc: Lexer: Failed to convert string to float in scanNumLiteral: invalid string\n";
                        exit(1);
                    }
                    catch(const std::out_of_range& e){
                        std::cout << "Text: " << current_text_no_underscore;
                        std::cout << "velc: Lexer: Failed to convert string to float in scanNumLiteral: out of range\n";
                        exit(1);
                    }
                    catch(...){
                        std::cout << "Text: " << current_text_no_underscore;
                        std::cout << "velc: Lexer: Failed to convert string to float in scanNumLiteral: unknown\n";
                        exit(1);
                    }
                    return {type, current_text, 0, (current_text_no_underscore.empty()) ? 0 : valuef, startline, startcol};
                }
                else{
                    TokenType type = TokenType::INT_LITERAL;
                    int64_t valuei;
                    try{
                        valuei = std::stoll(current_text_no_underscore);
                    }
                    catch(const std::invalid_argument& e){
                        std::cout << "Text: " << current_text_no_underscore;
                        std::cout << "velc: Lexer: Failed to convert string to int in scanNumLiteral: invalid string\n";
                        exit(1);
                    }
                    catch(const std::out_of_range& e){
                        std::cout << "Text: " << current_text_no_underscore;
                        std::cout << "velc: Lexer: Failed to convert string to int in scanNumLiteral: out of range\n";
                        exit(1);
                    }
                    catch(...){
                        std::cout << "Text: " << current_text_no_underscore;
                        std::cout << "velc: Lexer: Failed to convert string to int in scanNumLiteral: unknown\n";
                        exit(1);
                    }
                    return {type, current_text, (current_text_no_underscore.empty()) ? 0 : valuei, 0, startline, startcol};
                }
            }
            advance(source, i, line, col);
        }
        if(current_text.empty()){
            return {TokenType::INVALID_TOKEN, "INVALID", 0, 0, startline, startcol};
        }
        if(isFloat){
            double valuef;
            try{
                valuef = std::stod(current_text_no_underscore);
            }
            catch(const std::invalid_argument& e){
                std::cout << "Text: " << current_text_no_underscore;
                std::cout << "velc: Lexer: Failed to convert string to float in scanNumLiteral: invalid string\n";
                exit(1);
            }
            catch(const std::out_of_range& e){
                std::cout << "Text: " << current_text_no_underscore;
                std::cout << "velc: Lexer: Failed to convert string to float in scanNumLiteral: out of range\n";
                exit(1);
            }
            catch(...){
                std::cout << "Text: " << current_text_no_underscore;
                std::cout << "velc: Lexer: Failed to convert string to float in scanNumLiteral: unknown\n";
                exit(1);
            }
            double value = (current_text_no_underscore.empty()) ? 0.0 : valuef;
            return {TokenType::FLOAT_LITERAL, current_text, 0, value, startline, startcol};
        } else {
            int64_t valuei;
            try{
                valuei = std::stoll(current_text_no_underscore);
            }
            catch(const std::invalid_argument& e){
                std::cout << "Text: " << current_text_no_underscore;
                std::cout << "velc: Lexer: Failed to convert string to int in scanNumLiteral: invalid string\n";
                exit(1);
            }
            catch(const std::out_of_range& e){
                std::cout << "Text: " << current_text_no_underscore;
                std::cout << "velc: Lexer: Failed to convert string to int in scanNumLiteral: out of range\n";
                exit(1);
            }
            catch(...){
                std::cout << "Text: " << current_text_no_underscore;
                std::cout << "velc: Lexer: Failed to convert string to int in scanNumLiteral: unknown\n";
                exit(1);
            }
            int64_t value = (current_text_no_underscore.empty()) ? 0 : valuei;
            return {TokenType::INT_LITERAL, current_text, value, 0, startline, startcol};
        }
    }
    
    Token scanStringLiteral(const std::string& source, uint32_t& i, int& line, int& col){
        advance(source, i, line, col);
        int startLine = line;
        int startCol = col;
    
        std::string currentText;
    
        while(i < source.length()) {
            char c = source[i];
    
            if(c == '\"'){
                advance(source, i, line, col);
                return {TokenType::STRING_LITERAL, currentText, 0, 0, startLine, startCol};
            } 
            else if(c == '\\'){
                if(i + 1 >= source.length()) break;
                char nextChar = source[i + 1];
                switch(nextChar) {
                    case 'n': currentText += '\n'; break;
                    case 't': currentText += '\t'; break;
                    case 'r': currentText += '\r'; break;
                    case 'f': currentText += '\f'; break;
                    case 'b': currentText += '\b'; break;
                    case '\\': currentText += '\\'; break;
                    case '"': currentText += '"'; break;
                    case '\'': currentText += '\''; break;
                    default: currentText += nextChar; break;
                }
                advance(source, i, line, col);
                advance(source, i, line, col);
            } 
            else{
                currentText += c;
                advance(source, i, line, col);
            }
        }
    
        return {TokenType::STRING_LITERAL, currentText, 0, 0, startLine, startCol};
    }
    Token scanCharLiteral(const std::string& source, uint32_t& i, int& line, int& col){
        advance(source, i, line, col);
        int startLine = line;
        int startCol = col;
    
        char value = '\0';
        std::string text;
    
        char c = source[i];
    
        if(c == '\\'){
            if(i < source.length()-1){
                char next_char = source[i+1];
                switch(next_char){
                    case 'n': value = '\n'; break;
                    case 't': value = '\t'; break;
                    case 'r': value = '\r'; break;
                    case 'f': value = '\f'; break;
                    case 'b': value = '\b'; break;
                    case '\\': value = '\\'; break;
                    case '\'': value = '\''; break;
                    case '\"': value = '\"'; break;
                    default: value = next_char; break;
                }
                advance(source, i, line, col);
                advance(source, i, line, col);
            }
            else{
                value = '\\';
                advance(source, i, line, col);
            }
        }
        else{
            value = c;
            advance(source, i, line, col);
        }
    
        if(i < source.length() && source[i] == '\''){
            advance(source, i, line, col);
        }
    
        text += value;
        return {TokenType::CHAR_LITERAL, text, value, 0, startLine, startCol};
    }
    


    void scanComment_Single(const std::string& source, uint32_t& i, int& line, int& col){
        advance(source, i, line, col);
        advance(source, i, line, col);
        for(;i<source.length()-1;){
            if(source[i] != '\n' && source[i+1] != '\n'){
                advance(source, i, line, col);
            }
            else if(source[i] == '\n'){
                advance(source, i, line, col);
                return;
            }
            advance(source, i, line, col);
        }
    }
    void scanComment_Multi(const std::string& source, uint32_t& i, int& line, int& col){
        advance(source, i, line, col);
        advance(source, i, line, col);
        for(;i<source.length()-1;){
            if(source[i] == '}' && source[i+1] == '/'){
                advance(source, i, line, col);
                advance(source, i, line, col);
                return;
            }
            advance(source, i, line, col);
        }
    }
    Token scanAssembly_Single(const std::string& source, uint32_t& i, int& line, int& col){
        advance(source, i, line, col);
        advance(source, i, line, col);
        std::string current_text;
        for(;i<source.length()-1;){
            if(source[i] != '\n' && source[i+1] != '\n'){
                current_text += source[i];
                current_text += source[i+1];
                advance(source, i, line, col);
                advance(source, i, line, col);
            }
            else if(source[i] != '\n'){
                current_text += source[i];
                advance(source, i, line, col);
            }
            else{
                advance(source, i, line, col);
                return {TokenType::ASSEMBLY, current_text + "\n", 0, 0, line, col};
            }
        }
        return {TokenType::ASSEMBLY, current_text + "\n", 0, 0, line, col};
    }
    Token scanAssembly_Multi(const std::string& source, uint32_t& i, int& line, int& col){
        advance(source, i, line, col);
        advance(source, i, line, col);
        std::string current_text;
        for(;i<source.length()-1;){
            if(source[i] == '}' && source[i+1] == '%'){
                advance(source, i, line, col);
                advance(source, i, line, col);
                return {TokenType::ASSEMBLY, current_text + "\n", 0, 0, line, col};
            }
            current_text += source[i];
            advance(source, i, line, col);
        }
        return {TokenType::ASSEMBLY, current_text + "\n", 0, 0, line, col};
    }





}









