#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstdint>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

enum class TokenType{
    //special, but like more special than the special ones, like dio
    INVALID_TOKEN,

    //keywords
    BOOL_KEYWORD,
    INT8_KEYWORD,
    INT16_KEYWORD,
    INT32_KEYWORD,
    INT64_KEYWORD,
    UINT8_KEYWORD,
    UINT16_KEYWORD,
    UINT32_KEYWORD,
    UINT64_KEYWORD,
    CHAR_KEYWORD,
    FLOAT32_KEYWORD,
    FLOAT64_KEYWORD,
    STRING_KEYWORD,
    VOID_KEYWORD,
    POINTER_KEYWORD,

    IF_KEYWORD,
    ELSE_KEYWORD,
    WHILE_KEYWORD,
    FOR_KEYWORD,
    RETURN_KEYWORD,

    TRUE_KEYWORD,
    FALSE_KEYWORD,

    //identifiers and literals
    IDENTIFIER,
    INT_LITERAL,
    FLOAT_LITERAL,
    CHAR_LITERAL,
    STRING_LITERAL,
    ASSEMBLY,
    COMMENT,

    //punctuation
    SEMICOLON,  // ;
    COMMA,      // ,
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }

    
    //operators
    ASSIGN,               // =
    ADD_ASSIGN,           // +=
    SUB_ASSIGN,           // -=
    MUL_ASSIGN,           // *=
    DIV_ASSIGN,           // /=
    MOD_ASSIGN,           // %=
    AND_ASSIGN,           // &=
    OR_ASSIGN,            // |=
    XOR_ASSIGN,           // ^=
    LSHIFT_ASSIGN,        // <<=
    RSHIFT_ASSIGN,        // >>=
    NOT_ASSIGN,           // !~=

    ADD,                  // +
    SUB,                  // -
    MUL,                  // *
    DIV,                  // /
    MOD,                  // %

    EQ,                   // ==
    NEQ,                  // !=
    LT,                   // <
    GT,                   // >
    LTE,                  // <=
    GTE,                  // >=

    AND,                  // &
    ANDAND,               // &&
    OR,                   // |
    OROR,                 // ||
    XOR,                  // ^
    XORXOR,               // ^^
    LSHIFT,               // <<
    RSHIFT,               // >>
    SWAP,                 // $$

    //prefix
    NOT_LOGICAL,          // !
    NOT_BITWISE,          // !~
    INC,                  // ++
    DEC,                  // --

    //special
    END_OF_FILE,
};

struct Token{
    TokenType type;
    std::string text;
    int64_t value;
    double valuef;
    int line;
    int col;
};

namespace Lexer{
    std::vector<Token> tokenize(const std::string& source);
    void advance(const std::string& source, uint32_t& i, int& line, int& col);
    Token scanKeywordOrIdentifier(const std::string& source, uint32_t& i, int& line, int& col);
    TokenType checkKeywordOrIdentifier(const std::string& text);
    std::string KeywordToString(const TokenType& type);
    Token scanNumLiteral(const std::string& source, uint32_t& i, int& line, int& col);
    Token scanStringLiteral(const std::string& source, uint32_t& i, int& line, int& col);
    Token scanCharLiteral(const std::string& source, uint32_t& i, int& line, int& col);
    Token scanComment_Single(const std::string& source, uint32_t& i, int& line, int& col);
    Token scanComment_Multi(const std::string& source, uint32_t& i, int& line, int& col);
    Token scanAssembly_Single(const std::string& source, uint32_t& i, int& line, int& col);
    Token scanAssembly_Multi(const std::string& source, uint32_t& i, int& line, int& col);
}


#endif