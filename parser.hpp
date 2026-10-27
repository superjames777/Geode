#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include "tokenizer.hpp"

namespace Geode {

namespace parser {

using typenodes::ASTNode;
using typenodes::Token;
using typenodes::TokCursor;
using typenodes::Type;

std::unique_ptr<ASTNode> program(TokCursor &tokcurs);
std::unique_ptr<ASTNode> statement(TokCursor &tokcurs);
std::unique_ptr<ASTNode> comparison(TokCursor &tokcurs);
std::unique_ptr<ASTNode> expression(TokCursor &tokcurs);
std::unique_ptr<ASTNode> term(TokCursor &tokcurs);
std::unique_ptr<ASTNode> unary(TokCursor &tokcurs);
std::unique_ptr<ASTNode> exponent(TokCursor &tokcurs);
std::unique_ptr<ASTNode> factor(TokCursor &tokcurs);

std::unique_ptr<ASTNode> program(TokCursor &tokcurs) {
    std::unique_ptr<ASTNode> sequencenode = std::make_unique<ASTNode>(Type::Sequence);
    sequencenode->children.push_back(statement(tokcurs));
    while (tokcurs.peek().type == Type::Separator) {
        tokcurs.advance();
        if (tokcurs.peek().type != Type::End) {
            sequencenode->children.push_back(statement(tokcurs));
        }
    }
    return sequencenode;
}

std::unique_ptr<ASTNode> statement(TokCursor &tokcurs) {
    if (tokcurs.peek().type == Type::Identifier && tokcurs.look().type == Type::Assignment) {
        std::string varname = tokcurs.peek().text;
        tokcurs.advance();
        tokcurs.advance();
        std::unique_ptr<ASTNode> left = comparison(tokcurs);
        std::unique_ptr<ASTNode> variable = std::make_unique<ASTNode>(varname, std::move(left));
        return variable;
    } else if (tokcurs.peek().type == Type::Keyword) {
        if (tokcurs.peek().text == "print") {
            tokcurs.advance();
            std::unique_ptr<ASTNode> expr = comparison(tokcurs);
            std::unique_ptr<ASTNode> print = std::make_unique<ASTNode>(Type::Print, std::move(expr));
            return print;
        } else {
            throw std::runtime_error("Nonexistant keyword passed as a keyword.\n");
        }
    } else {
        return comparison(tokcurs);
    }
}

std::unique_ptr<ASTNode> comparison(TokCursor &tokcurs) {
    std::unique_ptr<ASTNode> left = expression(tokcurs);
    if (tokcurs.peek().type == Type::Equality || tokcurs.peek().type == Type::Less || tokcurs.peek().type == Type::Greater || tokcurs.peek().type == Type::LessEqual || tokcurs.peek().type == Type::GreaterEqual || tokcurs.peek().type == Type::NotEqual) {
        Type comptype = tokcurs.peek().type;
        tokcurs.advance();
        std::unique_ptr<ASTNode> right = expression(tokcurs);
        left = std::make_unique<ASTNode>(comptype, 0, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> expression(TokCursor &tokcurs) {
    std::unique_ptr<ASTNode> left = term(tokcurs);
    while (tokcurs.peek().type == Type::Plus || tokcurs.peek().type == Type::Minus) {
        Type opertype = tokcurs.peek().type;
        tokcurs.advance();
        std::unique_ptr<ASTNode> right = term(tokcurs);
        left = std::make_unique<ASTNode>(opertype, 0, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> term(TokCursor &tokcurs) {
    std::unique_ptr<ASTNode> left = unary(tokcurs);
    while (tokcurs.peek().type == Type::Multiply || tokcurs.peek().type == Type::Divide) {
        Type opertype = tokcurs.peek().type;
        tokcurs.advance();
        std::unique_ptr<ASTNode> right = unary(tokcurs);
        left = std::make_unique<ASTNode>(opertype, 0, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> unary(TokCursor &tokcurs) {
    std::unique_ptr<ASTNode> nodet;
    if (tokcurs.peek().type == Type::Minus) {
        tokcurs.advance();
        std::unique_ptr<ASTNode> right = unary(tokcurs);
        std::unique_ptr<ASTNode> left = std::make_unique<ASTNode>(Type::Number, 0, nullptr, nullptr);
        nodet = std::make_unique<ASTNode>(Type::Minus, 0, std::move(left), std::move(right));
    } else {
        nodet = exponent(tokcurs);
    }
    return nodet;
}

std::unique_ptr<ASTNode> exponent(TokCursor &tokcurs) {
    std::unique_ptr<ASTNode> left = factor(tokcurs);
    if (tokcurs.peek().type == Type::Exponent) {
        tokcurs.advance();
        std::unique_ptr<ASTNode> right = unary(tokcurs);
        left = std::make_unique<ASTNode>(Type::Exponent, 0, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> factor(TokCursor &tokcurs) {
    const Token &curtoken = tokcurs.peek();
    if (curtoken.type == Type::Number) {
        double val = std::stod(curtoken.text);
        tokcurs.advance();
        std::unique_ptr<ASTNode> numnode = std::make_unique<ASTNode>(Type::Number, val, nullptr, nullptr);
        return numnode;
    } else if (curtoken.type == Type::LParentheses) {
        tokcurs.advance();
        std::unique_ptr<ASTNode> retval = comparison(tokcurs);
        if (tokcurs.peek().type == Type::RParentheses) {
            tokcurs.advance();
            return retval;
        } else {
            throw std::runtime_error(") expected.\n");
        }
    } else if (curtoken.type == Type::Identifier) {
        std::string varname = curtoken.text;
        tokcurs.advance();
        std::unique_ptr<ASTNode> varnode = std::make_unique<ASTNode>(varname);
        return varnode;
    } else {
        throw std::runtime_error("Invalid token type.\n");
    }
}

} // namespace parser

} // namespace Geode
