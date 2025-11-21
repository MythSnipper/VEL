#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


enum class TokenType{
    Function,
    Typename,
    Identifier,
    Block,
    Statement,
    LeftParenthesis,
    RightParenthesis,
    LeftBracket,
    RightBracket,
};

struct Token{
    TokenType type;
    std::string text;
};



#endif