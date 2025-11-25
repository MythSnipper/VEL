#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType{
    //keywords
    INT8_KEYWORD,
    INT16_KEYWORD,
    INT32_KEYWORD,
    INT64_KEYWORD,
    UINT8_KEYWORD,
    UINT16_KEYWORD,
    UINT32_KEYWORD,
    UINT64_KEYWORD,
    CHAR_KEYWORD,
    FLOAT_KEYWORD,
    DOUBLE_KEYWORD,
    IF_KEYWORD,
    ELSE_KEYWORD,
    WHILE_KEYWORD,
    FOR_KEYWORD,
    RETURN_KEYWORD,

    //identifiers and literals
    IDENTIFIER,
    INT_LITERAL,
    FLOAT_LITERAL,
    CHAR_LITERAL,
    STRING_LITERAL,

    //punctuation
    SEMICOLON,  // ;
    COMMA,      // ,
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }

    
    //operators
    ASSIGN,     // =
    ADD_ASSIGN, // +=
    SUB_ASSIGN, // -=
    MUL_ASSIGN, // *=
    DIV_ASSIGN, // /=
    MOD_ASSIGN, // %=
    XOR_ASSIGN, // ^=

    ADD,        // +
    SUB,        // -
    MUL,        // *
    DIV,        // /
    MOD,        // %

    EQ,         // ==
    NEQ,        // !=
    LT,         // <
    GT,         // >
    LTE,        // <=
    GTE,        // >=

    AND,        // &
    ANDAND,     // &&
    OR,         // |
    OROR,       // ||
    XOR,        // ^
    LSHIFT,     // <<
    RSHIFT,     // >>

    //prefix
    NOT,        // !
    INC,        // ++
    DEC,        // --

    //special
    END_OF_FILE,
};

struct Token{
    TokenType type;
    std::string text;
    int64_t value;
    int line;
    int col;
};

namespace Lexer{
    std::vector<Token> tokenize(const std::string& source);
    void advance(const std::string& source, int& i, int& line, int& col);
    Token scanKeywordOrIdentifier(const std::string& source, int& i, int& line, int& col);
    TokenType checkKeywordOrIdentifier(const std::string& text);
    std::string KeywordToString(const TokenType& type);
    void scanComment_Single(const std::string& source, int& i, int& line, int& col);
    void scanComment_Multi(const std::string& source, int& i, int& line, int& col);
}


#endif