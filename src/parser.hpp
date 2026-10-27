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
std::unique_ptr<ASTNode> block(TokCursor &tokcurs);
std::unique_ptr<ASTNode> comparison(TokCursor &tokcurs);
std::unique_ptr<ASTNode> expression(TokCursor &tokcurs);
std::unique_ptr<ASTNode> term(TokCursor &tokcurs);
std::unique_ptr<ASTNode> unary(TokCursor &tokcurs);
std::unique_ptr<ASTNode> exponent(TokCursor &tokcurs);
std::unique_ptr<ASTNode> factor(TokCursor &tokcurs);

namespace helpers {

std::unique_ptr<ASTNode> statementlist(TokCursor &tokcurs, Type terminator);
void expect(TokCursor &tokcurs, Type ty, const std::string &msg);

std::unique_ptr<ASTNode> statementlist(TokCursor &tokcurs, Type terminator) {
    std::unique_ptr<ASTNode> sequencenode = std::make_unique<ASTNode>(Type::Sequence);
    sequencenode->children.push_back(statement(tokcurs));
    while (true) {
        if (tokcurs.peek().type == Type::Separator) {
            tokcurs.advance();
        } else if (sequencenode->children.back()->nodetype == Type::If || sequencenode->children.back()->nodetype == Type::While) {

        } else {
            break;
        }
        if (tokcurs.peek().type == terminator) {
            break;
        } else if (tokcurs.peek().type == Type::End) {
            throw std::runtime_error("} expected.\n");
        }
        sequencenode->children.push_back(statement(tokcurs));
    } 
    return sequencenode;
}

void expect(TokCursor &tokcurs, Type ty, const std::string &msg) {
    if (tokcurs.peek().type != ty) {
        throw std::runtime_error(msg);
    } else {
        tokcurs.advance();
    }
}

} // namespace helpers

using namespace helpers;

std::unique_ptr<ASTNode> program(TokCursor &tokcurs) {
    return statementlist(tokcurs, Type::End);
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
            expect(tokcurs, Type::LParentheses, "( expected.\n");
            std::unique_ptr<ASTNode> expr = comparison(tokcurs);
            expect(tokcurs, Type::RParentheses, ") expected.\n");
            std::unique_ptr<ASTNode> print = std::make_unique<ASTNode>(Type::Print, std::move(expr));
            return print;
        } else if (tokcurs.peek().text == "while") {
            tokcurs.advance();
            expect(tokcurs, Type::LParentheses, "( expected.\n");
            std::unique_ptr<ASTNode> resultwhile = comparison(tokcurs);
            expect(tokcurs, Type::RParentheses, ") expected.\n");
            std::unique_ptr<ASTNode> resultwblock = block(tokcurs);
            std::unique_ptr<ASTNode> whilenode = std::make_unique<ASTNode>(Type::While);
            whilenode.get()->children.push_back(std::move(resultwhile));
            whilenode.get()->children.push_back(std::move(resultwblock));
            return whilenode;
        } else if (tokcurs.peek().text == "if") {
            tokcurs.advance();
            expect(tokcurs, Type::LParentheses, "( expected.\n");
            std::unique_ptr<ASTNode> resultif = comparison(tokcurs);
            expect(tokcurs, Type::RParentheses, ") expected.\n");
            std::unique_ptr<ASTNode> resultiblock = block(tokcurs);
            std::unique_ptr<ASTNode> ifnode = std::make_unique<ASTNode>(Type::If);
            ifnode.get()->children.push_back(std::move(resultif));
            ifnode.get()->children.push_back(std::move(resultiblock));
            if (tokcurs.peek().type == Type::Keyword && tokcurs.peek().text == "else") {
                tokcurs.advance();
                std::unique_ptr<ASTNode> resultelseblock = block(tokcurs);
                ifnode.get()->children.push_back(std::move(resultelseblock));
            }
            return ifnode;
        } else {
            throw std::runtime_error("Nonexistant keyword passed as a keyword.\n");
        }
    } else {
        return comparison(tokcurs);
    }
}

std::unique_ptr<ASTNode> block(TokCursor &tokcurs) {
    expect(tokcurs, Type::LCBracket, "{ expected.\n");
    std::unique_ptr<ASTNode> result = statementlist(tokcurs, Type::RCBracket);
    expect(tokcurs, Type::RCBracket, "} expected.\n");
    return result;
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
