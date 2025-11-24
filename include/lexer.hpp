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
    STRING_LITERAL,
    CHAR_LITERAL,

    // Operators

    // Punctuation
    SEMICOLON,  // ;
    COMMA,      // ,
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }


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
        //comparison
    EQ,         // ==
    NEQ,        // !=
    LT,         // <
    GT,         // >
    LTE,        // <=
    GTE,        // >=
        //logic
    AND,        // &
    ANDAND,     // &&
    OR,         // |
    OROR,       // ||
    XOR,        // ^
    LSHIFT,     // <<
    RSHIFT,     // >>

    //prefix
    NEG,        // -
    NOT,        // !

    INC,        // ++
    DEC,        // --
/* dead logic for lexer
    LINC,       // ++
    LDEC,       // --

    //postfix
    RINC,       // ++
    RDEC,       // --

*/

    // Special
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
    Token scanKW_Identifier(const std::string& source, int& i, int& line, int& col);
    TokenType checkKW_identifier(const std::string& text);
    void scanComment_Single(const std::string& source, int& i, int& line, int& col);
    void scanComment_Multi(const std::string& source, int& i, int& line, int& col);
}


#endif