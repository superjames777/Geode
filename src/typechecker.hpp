#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cctype>
#include <stdexcept>
#include <cmath>

#include "parser.hpp"

namespace Geode {

namespace typechecker {

using typenodes::ASTNode;
using typenodes::Token;
using typenodes::TokCursor;
using typenodes::Type;
using typenodes::ValType;

ValType checktype(ASTNode *node, std::map<std::string, ValType> &nmap);

ValType checktype(ASTNode *node, std::map<std::string, ValType> &nmap) {
    if (node->nodetype == Type::Number) {
        if (std::round(node->value) == node->value) {
            return ValType::Int;
        } else {
            throw std::runtime_error("Cannot assign type float to int type.\n");
        }
    } else if (node->nodetype == Type::String) {
        return ValType::Str;
    } else if (node->nodetype == Type::Bool) {
        return ValType::Bool;
    } else if (node->nodetype == Type::Variable) {
        if (nmap.find(node->name) != nmap.end()) {
            return nmap.at(node->name);
        } else {
            throw std::runtime_error("Undefined variable.\n");
        }
    } else if (node->nodetype == Type::Plus) {
        if (checktype(node->left.get(), nmap) == ValType::Int && checktype(node->right.get(), nmap) == ValType::Int) {
            return ValType::Int;
        } else if (checktype(node->left.get(), nmap) == ValType::Str && checktype(node->right.get(), nmap) == ValType::Str) {
            return ValType::Str;
        } else {
            throw std::runtime_error("Incompatible types for +.\n");
        }
    } else if (node->nodetype == Type::Minus) {
        if (checktype(node->left.get(), nmap) == ValType::Int && checktype(node->right.get(), nmap) == ValType::Int) {
            return ValType::Int;
        } else {
            throw std::runtime_error("Incompatible types for -.\n");
        }
    } else if (node->nodetype == Type::Multiply) {
        if (checktype(node->left.get(), nmap) == ValType::Int && checktype(node->right.get(), nmap) == ValType::Int) {
            return ValType::Int;
        } else {
            throw std::runtime_error("Incompatible types for *.\n");
        }
    } else if (node->nodetype == Type::Divide) {
        if (checktype(node->left.get(), nmap) == ValType::Int && checktype(node->right.get(), nmap) == ValType::Int) {
            return ValType::Int;
        } else {
            throw std::runtime_error("Incompatible types for /.\n");
        }
    } else if (node->nodetype == Type::Exponent) {
        if (checktype(node->left.get(), nmap) == ValType::Int && checktype(node->right.get(), nmap) == ValType::Int) {
            return ValType::Int;
        } else {
            throw std::runtime_error("Incompatible types for ^.\n");
        }
    } else if (node->nodetype == Type::Equality || node->nodetype == Type::Less || node->nodetype == Type::Greater || node->nodetype == Type::LessEqual || node->nodetype == Type::GreaterEqual || node->nodetype == Type::NotEqual) {
        if (checktype(node->left.get(), nmap) == checktype(node->right.get(), nmap)) {
            return ValType::Bool;
        } else {
            throw std::runtime_error("Cannot compare different types.\n");
        }
    } else if (node->nodetype == Type::Declare) {
        if ((checktype(node->children[0].get(), nmap) != node->valtype) || (nmap.find(node->name) != nmap.end())) {
            throw std::runtime_error("Incompatible type.\n");
        } else {
            nmap[node->name] = node->valtype;
            return node->valtype;
        }
    } else if (node->nodetype == Type::Assign) {
        if (nmap.find(node->name) == nmap.end() || checktype(node->left.get(), nmap) != nmap.at(node->name)) {
            throw std::runtime_error("Incompatible type.\n");
        } else {
            return nmap.at(node->name);
        }
    } else if (node->nodetype == Type::If || node->nodetype == Type::While) {
        if (checktype(node->children[0].get(), nmap) == ValType::Bool) {
            checktype(node->children[1].get(), nmap);
            if (node->children.size() == 3) {
                checktype(node->children[2].get(), nmap);
            }
            return ValType::Int;
        } else {
            throw std::runtime_error("Invalid comparison.\n");
        }
    } else if (node->nodetype == Type::Print) {
        checktype(node->children[0].get(), nmap);
        return ValType::Int;
    } else if (node->nodetype == Type::Sequence) {
        for (const std::unique_ptr<ASTNode> &child : node->children) {
            checktype(child.get(), nmap);
        }
        return ValType::Int;
    } else {
        throw std::runtime_error("Invalid nodetype.\n");
    }
}

} // namespace typechecker

} // namespace Geode
