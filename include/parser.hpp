#ifndef PARSER_HPP
#define PARSER_HPP

#include <cstdint>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <lexer.hpp>

//-----------------------------------------------------------------------------------------------------------------------

enum class BuiltinType{
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,

    FLOAT32,
    FLOAT64,
    
    CHAR,
    STRING,

    VOID,
};

enum class LiteralType{
    INT,
    FLOAT,
    CHAR,
    STRING,
};

enum class PostfixOperator{
    INC,   // ++
    DEC    // --
};

enum class PrefixOperator{
    LOGICAL_NOT,    // !
    BITWISE_NOT,    // !~
    INC,            // ++
    DEC,            // --
    ADD,            // +
    NEG,            // -
};

enum class BinaryOperator{
    // arithmetic
    ADD,        // +
    SUB,        // -
    MUL,        // *
    DIV,        // /
    MOD,        // %

    // comparison
    EQ,         // ==
    NEQ,        // !=
    LT,         // <
    GT,         // >
    LTE,        // <=
    GTE,        // >=

    // bitwise
    AND,        // &
    OR,         // |
    XOR,        // ^
    LSHIFT,     // <<
    RSHIFT,     // >>

    // logical
    ANDAND,     // &&
    OROR        // ||
};

enum class AssignmentOperator{
    ASSIGN,        // =
    ADD_ASSIGN,    // +=
    SUB_ASSIGN,    // -=
    MUL_ASSIGN,    // *=
    DIV_ASSIGN,    // /=
    MOD_ASSIGN,    // %= 
    AND_ASSIGN,    // &=
    OR_ASSIGN,     // |=
    XOR_ASSIGN,    // ^=
    LSHIFT_ASSIGN, // <<=
    RSHIFT_ASSIGN, // >>=
    NOT_ASSIGN,    // !~=
    SWAP,          // ^^   I'll include it here as it is kind of an assignment operator, can be fixed in semantic analysis
};


//-----------------------------------------------------------------------------------------------------------------------


struct Type{
    bool isBuiltin;
    //BuiltinType is used because in function and variable declarations
    //the exact type must be used for semantic analysis and codegen
    BuiltinType builtinType; // valid if isBuiltin
    std::string name;        // user-defined type
};

//-----------------------------------------------------------------------------------------------------------------------

struct ASTNode{
    virtual ~ASTNode(){};
};

struct Declaration : virtual ASTNode{};
struct Expression : virtual ASTNode{};
struct Statement : virtual ASTNode{};


//-----------------------------------------------------------------------------------------------------------------------
struct Literal : Expression{
    LiteralType Type; //LiteralType resolves to BuiltinType during semantic analysis and codegen

    int64_t IntValue;
    double FloatValue;
    char CharValue;
    std::string StringValue;
};

struct Identifier : Expression{
    std::string Text;
    Identifier(){};
    Identifier(const std::string& text){
        Text = text;
    }
};

struct Call : Expression{
    std::unique_ptr<Identifier> Id;
    std::vector<std::unique_ptr<Expression>> Arguments;
};

struct PostfixOp : Expression{
    std::unique_ptr<Expression> Expr;
    PostfixOperator Op;
};

struct PrefixOp : Expression{
    PrefixOperator Op;
    std::unique_ptr<Expression> Expr;
};

struct BinaryOp : Expression{
    std::unique_ptr<Expression> Left;
    BinaryOperator Op;
    std::unique_ptr<Expression> Right;
};

struct AssignmentOp : Expression{
    std::unique_ptr<Identifier> Target;
    AssignmentOperator Op;
    std::unique_ptr<Expression> Modifier;
};
//-----------------------------------------------------------------------------------------------------------------------

struct Program : ASTNode{
    std::vector<std::unique_ptr<Declaration>> TopLevel;
};

struct Block : Statement{
    std::vector<std::unique_ptr<Statement>> Statements;
};

struct GlobalVariableDeclaration : Declaration{
    Type Typename;
    std::unique_ptr<Identifier> Id;
    std::unique_ptr<Expression> Expr; //Expression or nullptr
};

struct Function : Declaration{
    Type ReturnType;
    std::unique_ptr<Identifier> Id;
    std::vector<std::pair<Type, std::unique_ptr<Identifier>>> Parameters; //Typename, Identifier
    std::unique_ptr<Block> Body;
};
//-----------------------------------------------------------------------------------------------------------------------

struct VariableDeclaration : Statement{
    Type Typename;
    std::unique_ptr<Identifier> Id;
    std::unique_ptr<Expression> Expr; //Expression or nullptr
};

struct Return : Statement{
    std::unique_ptr<Expression> Expr;
};

struct ExpressionStatement : Statement{
    std::unique_ptr<Expression> Expr;
};

struct ElseIf{
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<Statement> Body; //Statement or Block
};

struct If : Statement {
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<Statement> Body; //Statement or Block
    std::vector<ElseIf> ElseIfs;
    std::unique_ptr<Statement> Else; //Statement or Block or nullptr
};

struct While : Statement{
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<Statement> Body; //Statement or Block or nullptr
};

struct For : Statement{
    std::unique_ptr<ASTNode> Initializer; //Expression or VariableDeclaration or nullptr
    std::unique_ptr<Expression> Condition; //Expression or nullptr
    std::unique_ptr<Expression> Modifier; //Expression or nullptr
    std::unique_ptr<Statement> Body; //Statement or Block or nullptr
};

struct Assembly : Statement, Declaration{
    std::string Text;
    Assembly(){};
    Assembly(const std::string& text){
        Text = text;
    }
};

struct Empty : Statement{
    // :3
};
//-----------------------------------------------------------------------------------------------------------------------



Program constructAST(const std::vector<Token>& tokenList);
Token getToken(const std::vector<Token>& tokenList, const int& index, const int& offset = 0);
void advance(const std::vector<Token>&, int& index, const int& numTokens);
bool isType(const Token& token, const TokenType& type);
bool isTypename(const Token& token);
BuiltinType convertType(const TokenType& type);

std::unique_ptr<Function> parseFunction(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<Assembly> parseAssembly(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<GlobalVariableDeclaration> parseGlobalVariableDeclaration(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<VariableDeclaration> parseVariableDeclaration(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<Block> parseBlock(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<Statement> parseStatement(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<Return> parseReturn(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<If> parseIf(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<While> parseWhile(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<For> parseFor(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<ExpressionStatement> parseExpressionStatement(const std::vector<Token>& tokenList, int& index);
std::unique_ptr<Expression> parseExpression(const std::vector<Token>& tokenList, int& index);






#endif