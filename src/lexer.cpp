#include <lexer.hpp>


namespace Lexer{
    std::vector<Token> tokenize(const std::string& source){
        std::vector<Token> token_list;

        Token current_token;
        int line = 1;
        int col = 1;

        for(int i=0;i<source.length();){
            char current_char = source[i];
            char next_char = (i != source.length()-1) ? source[i+1] : '\0';

            if(isalpha(current_char) || current_char == '_'){ //scan for keyword or identifier
                current_token = scanKW_Identifier(source, i, line, col);
                token_list.push_back(current_token);
            }
            else if(current_char == ';'){ //punctuation
                current_token = (Token){TokenType::SEMICOLON, ";", 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == ','){
                current_token = (Token){TokenType::COMMA, ",", 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '('){
                current_token = (Token){TokenType::LPAREN, "(", 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == ')'){
                current_token = (Token){TokenType::RPAREN, ")", 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '{'){
                current_token = (Token){TokenType::LBRACE, "{", 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '}'){
                current_token = (Token){TokenType::RBRACE, "}", 0, line, col};
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }









            //add rules for operators
            else if(current_char == '+'){
                if(next_char == '='){
                    current_token = (Token){TokenType::ADD_ASSIGN, "+=", 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '+'){
                    current_token = (Token){TokenType::INC, "++", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::ADD, "+", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '-'){
                if(next_char == '='){
                    current_token = (Token){TokenType::SUB_ASSIGN, "-=", 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '-'){
                    current_token = (Token){TokenType::DEC, "--", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::SUB, "-", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '*'){
                if(next_char == '='){
                    current_token = (Token){TokenType::MUL_ASSIGN, "*=", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::MUL, "*", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '/'){
                if(next_char == '='){
                    current_token = (Token){TokenType::DIV_ASSIGN, "/=", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::DIV, "/", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '%'){
                if(next_char == '='){
                    current_token = (Token){TokenType::MOD_ASSIGN, "%=", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::MOD, "%", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '='){
                if(next_char == '='){
                    current_token = (Token){TokenType::EQ, "==", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::ASSIGN, "=", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '!'){
                if(next_char == '='){
                    current_token = (Token){TokenType::NEQ, "!=", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::NOT, "!", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '<'){
                if(next_char == '='){
                    current_token = (Token){TokenType::LTE, "<=", 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '<'){
                    current_token = (Token){TokenType::LSHIFT, "<<", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::LT, "<", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '>'){
                if(next_char == '='){
                    current_token = (Token){TokenType::GTE, ">=", 0, line, col};
                    advance(source, i, line, col);
                }
                else if(next_char == '>'){
                    current_token = (Token){TokenType::RSHIFT, ">>", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::GT, ">", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '&'){
                if(next_char == '&'){
                    current_token = (Token){TokenType::ANDAND, "&&", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::AND, "&", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '|'){
                if(next_char == '|'){
                    current_token = (Token){TokenType::OROR, "||", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::OR, "|", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '^'){
                if(next_char == '='){
                    current_token = (Token){TokenType::XOR_ASSIGN, "^=", 0, line, col};
                    advance(source, i, line, col);
                }
                else{
                    current_token = (Token){TokenType::XOR, "^", 0, line, col};
                }
                token_list.push_back(current_token);
                advance(source, i, line, col);
            }
            else if(current_char == '/' && next_char == '/'){ //single line comment
                scanComment_Single(source, i, line, col);
            }
            else if(current_char == '/' && next_char == '*'){ //multi line comment
                scanComment_Multi(source, i, line, col);
            }
            else if(current_char == ' ' || current_char == '\n' || current_char == '\t'){
                advance(source, i, line, col);
            }
            else{
                std::cout << "Invalid character: " << current_char << " ";
                if(next_char != NULL){
                    std::cout << next_char;
                }
                else{
                    std::cout << "null";
                }
                std::cout << std::endl;
                advance(source, i, line, col);
            }
        }

        token_list.push_back({TokenType::END_OF_FILE, "\0", -1, line, col});
        return token_list;
    }
    void advance(const std::string& source, int& i, int& line, int& col){
        if(source[i] == '\n'){
            col = 1;
            line++;
        }
        else{
            col++;
        }
        i++;
    }
    Token scanKW_Identifier(const std::string& source, int& i, int& line, int& col){
        std::string current_text;

        for(;i<source.length();){
            char current_char = source[i];
            if(isalnum(current_char) || current_char == '_'){
                current_text += current_char;
            }
            else{
                TokenType type = checkKW_identifier(current_text);
                return {type, current_text, 0, line, col};
            }
            advance(source, i, line, col);
        }


    }
    TokenType checkKW_identifier(const std::string& text){
        std::unordered_map<std::string, TokenType> map = {
            {"int8", TokenType::INT8_KEYWORD},
            {"int16", TokenType::INT16_KEYWORD},
            {"int32", TokenType::INT32_KEYWORD},
            {"int64", TokenType::INT64_KEYWORD},
            {"uint8", TokenType::UINT8_KEYWORD},
            {"uint16", TokenType::UINT16_KEYWORD},
            {"uint32", TokenType::UINT32_KEYWORD},
            {"uint64", TokenType::UINT64_KEYWORD},
            {"char", TokenType::CHAR_KEYWORD},
            {"float", TokenType::FLOAT_KEYWORD},
            {"double", TokenType::DOUBLE_KEYWORD},
            {"if", TokenType::IF_KEYWORD},
            {"else", TokenType::ELSE_KEYWORD},
            {"while", TokenType::WHILE_KEYWORD},
            {"for", TokenType::FOR_KEYWORD},
            {"return", TokenType::RETURN_KEYWORD}
        };
        return (map.find(text) != map.end()) ? map[text] : TokenType::IDENTIFIER;


    }
    void scanComment_Single(const std::string& source, int& i, int& line, int& col){
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
    void scanComment_Multi(const std::string& source, int& i, int& line, int& col){
        advance(source, i, line, col);
        advance(source, i, line, col);
        for(;i<source.length()-1;){
            if(source[i] == '*' && source[i+1] == '/'){
                advance(source, i, line, col);
                advance(source, i, line, col);
                return;
            }
            advance(source, i, line, col);
        }
    }





};









