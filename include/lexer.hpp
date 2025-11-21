#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum class TokenType{
    //keywords
    INT_KEYWORD,
    CHAR_KEYWORD,
    DOUBLE_KEYWORD,
    IF_KEYWORD,
    ELSE_KEYWORD,
    WHILE_KEYWORD,
    FOR_KEYWORD,
    RETURN_KEYWORD,

    // Identifiers and literals
    IDENTIFIER,
    INT_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,

    // Operators

    //assignment
    ASSIGN,     // =
    ADD_ASSIGN, // +=
    SUB_ASSIGN, // -=
    MUL_ASSIGN, // *=
    DIV_ASSIGN, // /=
    MOD_ASSIGN, // %=
    XOR_ASSIGN, // ^=

    //binary
    ADD,        // +
    SUB,        // -
    MUL,        // *
    DIV,        // /
    MOD,        // %
    LT,         // <
    GT,         // >
    LTE,        // <=
    GTE,        // >=
    NEQ,        // !=
    AND,        // &
    ANDAND,     // &&
    OR,         // |
    OROR,       // ||

    NOT,        // !
    
    EQ,         // ==

    // Punctuation
    SEMICOLON,  // ;
    COMMA,      // ,
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }

    // Special
    END_OF_FILE,
};

struct Token{
    TokenType type;

};



#endif