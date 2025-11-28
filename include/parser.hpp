#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

struct ASTNode{
    virtual ~ASTNode(){};
};


struct Program : ASTNode{
    std::vector<std::unique_ptr<ASTNode>> TopLevel; //Function or VarDecl
};

struct VarDecl : ASTNode{
    std::string Typename;
    std::string Identifier;
    std::unique_ptr<ASTNode> Initializer; //Expression or null
};

struct Function : ASTNode{
    std::string ReturnType;
    std::string Identifier;
    std::vector<std::pair<std::string, std::string>> Parameters; // pairs of (Typename, Identifier)
    std::unique_ptr<Block> Body;
};








#endif