#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cctype>
#include <stdexcept>
#include <cmath>

namespace Geode {

namespace typenodes {

extern std::string TYPESTRS[18];

enum class Type { 
    Plus = 0, 
    Minus = 1, 
    Multiply = 2, 
    Divide = 3, 
    Exponent = 4, 
    Number = 5, 
    LParentheses = 6, 
    RParentheses = 7,
    Unknown = 8,
    End = 9,
    Identifier = 10,
    Assignment = 11,
    Variable = 12,
    Assign = 13,
    Separator = 14,
    Sequence = 15,
    Keyword = 16,
    Print = 17
};

std::string TYPESTRS[18] = { 
    "Plus", 
    "Minus", 
    "Multiply", 
    "Divide", 
    "Exponent",
    "Number",
    "LParentheses",
    "RParentheses",
    "Unknown",
    "End",
    "Identifier",
    "Assignment",
    "Variable",
    "Assign",
    "Separator",
    "Sequence",
    "Keyword",
    "Print"
};

std::map<std::string, Type> KEYWORDS = {
    {"print", Type::Keyword}
};

struct Token {
    Type type;
    std::string text;
};

struct ASTNode {
    Type nodetype;
    std::string name;
    double value;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    std::vector<std::unique_ptr<ASTNode>> children;

    ASTNode(Type ntype, double val, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r) : 
            nodetype(ntype), 
            value(val), 
            left(std::move(l)), 
            right(std::move(r))
    {}

    explicit ASTNode(const std::string &nname) :
            nodetype(Type::Variable),
            name(nname),
            value(0),
            left(nullptr),
            right(nullptr)
    {}

    ASTNode(const std::string &nname, std::unique_ptr<ASTNode> l) :
            nodetype(Type::Assign),
            name(nname),
            value(0),
            left(std::move(l)),
            right(nullptr)
    {}

    explicit ASTNode(Type ntype) :
            nodetype(ntype),
            value(0),
            left(nullptr),
            right(nullptr)
    {}

    ASTNode(Type ntype, std::unique_ptr<ASTNode> child) :
            nodetype(ntype),
            value(0),
            left(nullptr),
            right(nullptr)
    {
        children.push_back(std::move(child));
    }
};

struct TokCursor {
    std::vector<Token> tokenvector;
    size_t vectorpos;

    TokCursor(const std::vector<Token> &tokvect, size_t vectpos) : 
              tokenvector(tokvect), 
              vectorpos(vectpos) 
    {}

    const Token &peek() const {
        return tokenvector[vectorpos];
    }

    void advance() {
        if (vectorpos + 1 < tokenvector.size()) {
            vectorpos++;
        }
        return;
    }

    const Token &look() const {
        if (vectorpos + 1 < tokenvector.size()) {
            return tokenvector[vectorpos + 1];
        }
        return tokenvector[vectorpos];
    }
};

std::string gettypestr(Type type) {
    return TYPESTRS[static_cast<int>(type)];
}

} // namespace typenodes

} // namespace Geode
