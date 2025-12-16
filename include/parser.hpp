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
    BOOL,
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

    POINTER,
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
    BITWISE_AND,        // &
    BITWISE_OR,         // |
    BITWISE_XOR,        // ^
    LSHIFT,     // <<
    RSHIFT,     // >>

    // logical
    LOGICAL_AND,     // &&
    LOGICAL_OR,       // ||
    LOGICAL_XOR,     // ^^



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
    SWAP,          // $$
};


void printIndent(int level);

std::string toString(BuiltinType t);
std::string toString(LiteralType t);
std::string toString(PostfixOperator t);
std::string toString(PrefixOperator t);
std::string toString(BinaryOperator t);


//-----------------------------------------------------------------------------------------------------------------------

struct Type{
    bool isBuiltin;
    //BuiltinType is used because in function and variable declarations
    //the exact type must be used for semantic analysis and codegen
    BuiltinType builtinType; // valid if isBuiltin
    std::string name;        // user-defined type

    bool isPointer = false;
    std::unique_ptr<Type> pointerType = nullptr;
};

//-----------------------------------------------------------------------------------------------------------------------

struct ASTNode{
    virtual ~ASTNode(){};
    virtual void print(int) const{};
};

struct Declaration : virtual ASTNode{};
struct Expression : virtual ASTNode{};
struct Statement : virtual ASTNode{};

//-----------------------------------------------------------------------------------------------------------------------
//Analyzer stuff
enum class SymbolType{
    Variable,
    Function,
    Parameter,
};
struct Symbol{
    SymbolType Segregator;
    std::string Id;
    BuiltinType Type;
    std::vector<BuiltinType> ParamTypes;

    //may be used for later
    ASTNode* decl = nullptr;
};
struct SymbolTable{
    std::unordered_map<std::string, Symbol> Symbols;
    SymbolTable* parent = nullptr; //links to outer symbol table
};
/* nullptr <- Program <- Function
                            ^
                            | <- Block
                            | <- For
                            | <- If(single statement)
                            | <- While(single statement)
                            | 
                            |
                            |

*/

//-----------------------------------------------------------------------------------------------------------------------
struct Literal : Expression{
    LiteralType Type; //LiteralType resolves to BuiltinType during semantic analysis and codegen

    int64_t IntValue;
    double FloatValue;
    char CharValue;
    std::string StringValue;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Literal: ";
        switch(Type){
            case LiteralType::INT:
            std::cout << IntValue;break;
            case LiteralType::FLOAT:
            std::cout << FloatValue;break;
            case LiteralType::CHAR:
            std::cout << CharValue;break;
            case LiteralType::STRING:
            std::cout << StringValue;break;
        }
        std::cout << "\n";
    }
};

struct Identifier : Expression{
    std::string Text;
    Identifier(){};
    Identifier(const std::string& text){
        Text = text;
    }

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Identifier: ";
        std::cout << Text;
        std::cout << "\n";
    }
};

struct Call : Expression{
    std::unique_ptr<Identifier> Id;
    std::vector<std::unique_ptr<Expression>> Arguments;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Call: \n";
        printIndent(level+1);
        Id->print();
        for(uint32_t i=0;i<Arguments.size();i++){
            printIndent(level+1);
            std::cout << "Argument " << i+1 << ": \n";
            Arguments[i]->print(level+2);
        }
    }
};

struct PostfixOp : Expression{
    std::unique_ptr<Expression> Expr;
    PostfixOperator Op;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "PostfixOp: " << toString(Op) << "\n";
        Expr->print(level + 1);
    }
};

struct PrefixOp : Expression{
    PrefixOperator Op;
    std::unique_ptr<Expression> Expr;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "PrefixOp: " << toString(Op) << "\n";
        Expr->print(level + 1);
    }
};

struct BinaryOp : Expression{
    std::unique_ptr<Expression> Left;
    BinaryOperator Op;
    std::unique_ptr<Expression> Right;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "BinaryOp: " << toString(Op) << "\n";
        Left->print(level + 1);
        Right->print(level + 1);
    }
};

struct AssignmentOp : Expression{
    std::unique_ptr<Expression> Left;
    BinaryOperator Op;
    std::unique_ptr<Expression> Right;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "AssignmentOp: " << toString(Op) << "\n";
        Left->print(level + 1);
        Right->print(level + 1);
    }
};
//-----------------------------------------------------------------------------------------------------------------------

struct Program : ASTNode{
    std::vector<std::unique_ptr<Declaration>> TopLevel;

    std::unique_ptr<SymbolTable> Table;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Program: \n";
        for(uint32_t i=0;i<TopLevel.size();i++){
            printIndent(level+1);
            std::cout << "Declaration " << i+1 << ": \n";
            TopLevel[i]->print(level+2);
        }
    }
};

struct Block : Statement{
    std::vector<std::unique_ptr<Statement>> Statements;

    std::unique_ptr<SymbolTable> Table;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Block: \n";
        for(uint32_t i=0;i<Statements.size();i++){
            printIndent(level+1);
            std::cout << "Statement " << i+1 << ": \n";
            Statements[i]->print(level+2);
        }
    }
};

struct GlobalVariableDeclaration : Declaration{
    Type Typename;
    std::unique_ptr<Identifier> Id;
    std::unique_ptr<Expression> Expr; //Expression or nullptr

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Global Variable: \n";
        printIndent(level+1);
        std::cout << "Type: ";
        std::cout << toString(Typename.builtinType);
        std::cout << "\n";
        Id->print(level + 1);
        if(Expr){
            printIndent(level+1);
            std::cout << "Expression: \n";
            Expr->print(level + 1);
        }
    }
};

struct Function : Declaration{
    Type ReturnType;
    std::unique_ptr<Identifier> Id;
    std::vector<std::pair<Type, std::unique_ptr<Identifier>>> Parameters; //Typename, Identifier
    std::unique_ptr<Statement> Body;

    std::unique_ptr<SymbolTable> Table;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Function: \n";
        printIndent(level+1);
        std::cout << "Return Type: ";
        std::cout << toString(ReturnType.builtinType);
        std::cout << "\n";
        Id->print(level + 1);
        for(uint32_t i=0;i<Parameters.size();i++){
            printIndent(level+1);
            std::cout << "Parameter " << i+1 << ": \n";
            printIndent(level+2);
            std::cout << "Type: ";
            std::cout << toString(Parameters[i].first.builtinType);
            std::cout << "\n";
            Parameters[i].second->print(level+2);
            std::cout << "\n";
        }
        Body->print(level + 1);
    }
};
//-----------------------------------------------------------------------------------------------------------------------

struct VariableDeclaration : Statement{
    Type Typename;
    std::unique_ptr<Identifier> Id;
    std::unique_ptr<Expression> Expr; //Expression or nullptr

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Variable Declaration: \n";
        printIndent(level+1);
        std::cout << "Type: ";
        std::cout << toString(Typename.builtinType);
        std::cout << "\n";
        Id->print(level + 1);
        if(Expr){
            printIndent(level+1);
            std::cout << "Expression: \n";
            Expr->print(level+1);
        }
    }
};

struct Return : Statement{
    std::unique_ptr<Expression> Expr;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Return: \n";
        if(Expr){
            printIndent(level+1);
            std::cout << "Expression: \n";
            Expr->print(level+1);
        }
    }
};

struct ExpressionStatement : Statement{
    std::unique_ptr<Expression> Expr;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "ExpressionStatement: \n";
        if(Expr){
            printIndent(level+1);
            std::cout << "Expression: \n";
            Expr->print(level+1);
        }
    }
};

struct ElseIf{
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<Statement> Body; //Statement or Block

    std::unique_ptr<SymbolTable> Table; //only if Body is not a Block

    void print(int level = 0) const{
        printIndent(level);
        std::cout << "ElseIf: \n";
        Condition->print(level + 1);
        Body->print(level + 1);
    }
};

struct If : Statement {
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<Statement> Body; //Statement or Block
    std::vector<ElseIf> ElseIfs;
    std::unique_ptr<Statement> Else; //Statement or Block or nullptr

    std::unique_ptr<SymbolTable> TableIf; //only if Body is not a Block

    std::unique_ptr<SymbolTable> TableElse; //only if Else is not a Block

    
    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "If: \n";
        printIndent(level+1);
        std::cout << "Condition: \n";
        Condition->print(level + 1);
        printIndent(level+1);
        std::cout << "Body: \n";
        Body->print(level + 1);
        for(const auto& elseIf : ElseIfs){
            elseIf.print(level + 1);
        }
        if(Else){
            printIndent(level);
            std::cout << "Else: \n";
            Else->print(level + 1);
        }
    }
};

struct While : Statement{
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<Statement> Body; //Statement or Block or nullptr

    std::unique_ptr<SymbolTable> Table; //only if Body is not a Block


    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "While: \n";
        printIndent(level+1);
        std::cout << "Condition: \n";
        Condition->print(level + 1);
        printIndent(level+1);
        std::cout << "Body: \n";
        Body->print(level + 1);
    }
};

struct For : Statement{
    std::unique_ptr<ASTNode> Initializer; //Expression or VariableDeclaration or nullptr
    std::unique_ptr<Expression> Condition; //Expression or nullptr
    std::unique_ptr<Expression> Modifier; //Expression or nullptr
    std::unique_ptr<Statement> Body; //Statement or Block or nullptr

    std::unique_ptr<SymbolTable> Table;

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "For: \n";
        if(Initializer){
            printIndent(level+1);
            std::cout << "Initializer: \n";
            Initializer->print(level+1);
        }
        if(Condition){
            printIndent(level+1);
            std::cout << "Condition: \n";
            Condition->print(level + 1);
        }
        if(Modifier){
            printIndent(level+1);
            std::cout << "Modifier: \n";
            Modifier->print(level+1);
        }
        if(Body){
            printIndent(level+1);
            std::cout << "Body: \n";
            Body->print(level+1);
        }
    }
};

struct Assembly : Statement, Declaration{
    std::string Text;
    Assembly(){};
    Assembly(const std::string& text){
        Text = text;
    }

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Assembly: " << Text << "\n";
    }
};

struct Comment : Statement, Declaration{
    std::string Text;
    Comment(){};
    Comment(const std::string& text){
        Text = text;
    }

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Comment: " << Text << "\n";
    }
};

struct Empty : Statement{
    // :3

    void print(int level = 0) const override{
        printIndent(level);
        std::cout << "Empty\n";
    }
};
//-----------------------------------------------------------------------------------------------------------------------

namespace Parser{
    Program constructAST(const std::vector<Token>& tokenList);
    Token getToken(const std::vector<Token>& tokenList, const int& index, const int& offset = 0);
    void advance(const std::vector<Token>&, int& index, const int& numTokens);
    bool isType(const Token& token, const TokenType& type);
    bool isLiteral(const Token& token);
    bool isTypename(const Token& token);
    BuiltinType toBuiltinType(const TokenType& type);

    Type getType(const std::vector<Token>& tokenList, int& index);

    
    std::unique_ptr<Function> parseFunction(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<Assembly> parseAssembly(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<Comment> parseComment(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<GlobalVariableDeclaration> parseGlobalVariableDeclaration(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<VariableDeclaration> parseVariableDeclaration(const std::vector<Token>& tokenList, int& index, const TokenType& stopType = TokenType::SEMICOLON);
    std::unique_ptr<Block> parseBlock(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<Statement> parseStatement(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<Return> parseReturn(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<If> parseIf(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<While> parseWhile(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<For> parseFor(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<ExpressionStatement> parseExpressionStatement(const std::vector<Token>& tokenList, int& index, const TokenType& stopType = TokenType::SEMICOLON);
    
    int getPrecedence(const TokenType& type, const uint8_t& mode = 0);
    std::unique_ptr<Expression> parseExpression(const std::vector<Token>& tokenList, int& index, int minPrecedence = 0);
    std::unique_ptr<Expression> parsePrefixExpression(const std::vector<Token>& tokenList, int& index);
    std::unique_ptr<Expression> parsePostfixExpression(const std::vector<Token>& tokenList, int& index, std::unique_ptr<Expression> left);


    
}

#endif