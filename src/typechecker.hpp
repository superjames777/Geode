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
#include "scope.hpp"

namespace Geode {

namespace typechecker {

using typenodes::ASTNode;
using typenodes::Token;
using typenodes::TokCursor;
using typenodes::Type;
using typenodes::ValType;
using Geode::scope::scopestack;

ValType checktype(ASTNode *node, scopestack<ValType> &scopemap);

ValType checktype(ASTNode *node, scopestack<ValType> &scopemap) {
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
        ValType *found = scopemap.find(node->name);
        if (found != nullptr) {
            return *found;
        } else {
            throw std::runtime_error("Undefined variable.\n");
        }
    } else if (node->nodetype == Type::Plus) {
        ValType left = checktype(node->left.get(), scopemap);
        ValType right = checktype(node->right.get(), scopemap);
        if (left == ValType::Int && right == ValType::Int) {
            return ValType::Int;
        } else if (left == ValType::Str && right == ValType::Str) {
            return ValType::Str;
        } else {
            throw std::runtime_error("Incompatible types for +.\n");
        }
    } else if (node->nodetype == Type::Minus) {
        if (checktype(node->left.get(), scopemap) == ValType::Int && checktype(node->right.get(), scopemap) == ValType::Int) {
            return ValType::Int;
        } else {
            throw std::runtime_error("Incompatible types for -.\n");
        }
    } else if (node->nodetype == Type::Multiply) {
        if (checktype(node->left.get(), scopemap) == ValType::Int && checktype(node->right.get(), scopemap) == ValType::Int) {
            return ValType::Int;
        } else {
            throw std::runtime_error("Incompatible types for *.\n");
        }
    } else if (node->nodetype == Type::Divide) {
        if (checktype(node->left.get(), scopemap) == ValType::Int && checktype(node->right.get(), scopemap) == ValType::Int) {
            return ValType::Int;
        } else {
            throw std::runtime_error("Incompatible types for /.\n");
        }
    } else if (node->nodetype == Type::Exponent) {
        if (checktype(node->left.get(), scopemap) == ValType::Int && checktype(node->right.get(), scopemap) == ValType::Int) {
            return ValType::Int;
        } else {
            throw std::runtime_error("Incompatible types for ^.\n");
        }
    } else if (node->nodetype == Type::Equality || node->nodetype == Type::Less || node->nodetype == Type::Greater || node->nodetype == Type::LessEqual || node->nodetype == Type::GreaterEqual || node->nodetype == Type::NotEqual) {
        if (checktype(node->left.get(), scopemap) == checktype(node->right.get(), scopemap)) {
            return ValType::Bool;
        } else {
            throw std::runtime_error("Cannot compare different types.\n");
        }
    } else if (node->nodetype == Type::Declare) {
        if (checktype(node->children[0].get(), scopemap) != node->valtype) {
            throw std::runtime_error("Non-matching types.\n");
        } else if (scopemap.checkinner(node->name)) {
            throw std::runtime_error("Cannot redeclare variable.\n");
        } else {
            scopemap.innerdeclare(node->name, node->valtype);
            return node->valtype;
        }
    } else if (node->nodetype == Type::Assign) {
        ValType *found = scopemap.find(node->name);
        if (found == nullptr || checktype(node->left.get(), scopemap) != *found) {
            throw std::runtime_error("Incompatible type.\n");
        } else {
            return *found;
        }
    } else if (node->nodetype == Type::If || node->nodetype == Type::While) {
        if (checktype(node->children[0].get(), scopemap) == ValType::Bool) {
            checktype(node->children[1].get(), scopemap);
            if (node->children.size() == 3) {
                checktype(node->children[2].get(), scopemap);
            }
            return ValType::Int;
        } else {
            throw std::runtime_error("Invalid comparison.\n");
        }
    } else if (node->nodetype == Type::Print) {
        checktype(node->children[0].get(), scopemap);
        return ValType::Int;
    } else if (node->nodetype == Type::Sequence) {
        for (const std::unique_ptr<ASTNode> &child : node->children) {
            checktype(child.get(), scopemap);
        }
        return ValType::Int;
    } else if (node->nodetype == Type::Block) {
        scopemap.push();
        for (const std::unique_ptr<ASTNode> &child : node->children) {
            checktype(child.get(), scopemap);
        }
        scopemap.pop();
        return ValType::Int;
    } else {
        throw std::runtime_error("Invalid nodetype.\n");
    }
}

} // namespace typechecker

} // namespace Geode
