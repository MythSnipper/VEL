#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

struct Type{
    bool isBuiltin;
    LiteralType builtinType; // valid if isBuiltin
    std::string name;        // user-defined type
};


struct ASTNode{
    virtual ~ASTNode(){};
};

struct Declaration : ASTNode{};
struct Expression : ASTNode{};
struct Statement : ASTNode{};

struct Program : ASTNode{
    std::vector<std::unique_ptr<Declaration>> TopLevel;
};

struct VariableDeclaration : Declaration{
    Type Typename;
    std::string Identifier;
    std::unique_ptr<Expression> Expression; //Expression or nullptr
};

struct Function : Declaration{
    Type ReturnType;
    std::string Identifier;
    std::vector<std::pair<std::string, std::string>> Parameters; //Typename, Identifier
    std::unique_ptr<Block> Body; 
};


struct Block : Statement{
    std::vector<std::unique_ptr<Statement>> Statements;
};

struct Return : Statement{
    std::unique_ptr<Expression> Expression;
};

struct ExpressionStatement : Statement{
    std::unique_ptr<Expression> Expression;
};

struct If : Statement {
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<ASTNode> Body; //Statement or Block
    struct ElseIf{
        std::unique_ptr<Expression> Condition;
        std::unique_ptr<ASTNode> Body; //Statement or Block
    };
    std::vector<ElseIf> ElseIfs;
    std::unique_ptr<ASTNode> Else; //Statement or Block or nullptr
};

struct While : Statement{
    std::unique_ptr<Expression> Condition;
    std::unique_ptr<ASTNode> Body; //Statement or Block or nullptr
};

struct For : Statement{
    std::unique_ptr<ASTNode> Initializer; //Expression or VariableDeclaration or nullptr
    std::unique_ptr<Expression> Condition; //Expression or nullptr
    std::unique_ptr<Expression> Modifier; //Expression or nullptr
    std::unique_ptr<ASTNode> Body; //Statement or Block or nullptr
};

struct Empty : Statement{

};


struct Literal : Expression{
    LiteralType Type;

    std::string Text;
    int64_t IntValue;
    double FloatValue;
};

struct Identifier : Expression{
    std::string Text;
};

struct Call : Expression{
    std::unique_ptr<Identifier> Identifier;
    std::vector<std::unique_ptr<Expression>> Arguments;
};

struct PostfixOp : Expression{
    std::unique_ptr<Expression> Expression;
    PostfixOperator Op;
};

struct PrefixOp : Expression{
    PrefixOperator Op;
    std::unique_ptr<Expression> Expression;
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

enum class LiteralType{

};

enum class PostfixOperator{

};

enum class PrefixOperator{

};

enum class BinaryOperator{

};

enum class AssignmentOperator{

};







#endif