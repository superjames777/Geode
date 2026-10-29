#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include "typechecker.hpp"

namespace Geode {

namespace evaluator {

using typenodes::ASTNode;
using typenodes::Type;
using typenodes::Value;
using typenodes::ValType;

Value evaluate(ASTNode *head, std::map<std::string, Value> &varmap);
int valuecmp(const Value &left, const Value &right);

int valuecmp(const Value &left, const Value &right) {
    if (left.vtype != right.vtype) {
        throw std::runtime_error("Cannot compare different types.\n");
    }

    switch (left.vtype) {
        case ValType::Int:
            return (left.val > right.val) - (left.val < right.val);
        case ValType::Str:
            return left.strval.compare(right.strval);
        case ValType::Bool:
            return static_cast<int>(left.boolval) - static_cast<int>(right.boolval);
        default:
            throw std::runtime_error("Invalid value type.\n");
    }
}

Value evaluate(ASTNode *head, std::map<std::string, Value> &varmap) {
    if (head == nullptr) {
        throw std::runtime_error("Invalid head.\n");
    } else if (head->nodetype == Type::Number) {
        return Value(ValType::Int, static_cast<long long>(head->value));
    } else if (head->nodetype == Type::String) {
        return Value(ValType::Str, head->name);
    } else if (head->nodetype == Type::Bool) {
        return Value(ValType::Bool, static_cast<bool>(head->value));
    } else if (head->nodetype == Type::Variable) {
        auto variable = varmap.find(head->name);
        if (variable != varmap.end()) {
            return variable->second;
        }
        throw std::runtime_error("Undefined variable.\n");
    } else if (head->nodetype == Type::Declare) {
        Value value = evaluate(head->children[0].get(), varmap);
        varmap.emplace(head->name, value);
        return value;
    } else if (head->nodetype == Type::Assign) {
        Value varval = evaluate(head->left.get(), varmap);
        varmap.at(head->name) = varval;
        return varval;
    } else if (head->nodetype == Type::Sequence) {
        if (head->children.empty()) {
            throw std::runtime_error("Cannot evaluate an empty sequence.\n");
        }
        Value result = evaluate(head->children.front().get(), varmap);
        for (size_t i = 1; i < head->children.size(); i++) {
            result = evaluate(head->children[i].get(), varmap);
        }
        return result;
    } else if (head->nodetype == Type::Print) {
        Value toprint = evaluate(head->children[0].get(), varmap);
        switch (toprint.vtype) {
            case ValType::Int:
            std::cout << toprint.val;
            break;
            case ValType::Str:
            std::cout << toprint.strval;
            break;
            case ValType::Bool:
            std::cout << (toprint.boolval ? "true" : "false");
            break;
            default:
            throw std::runtime_error("Invalid value type.\n");
        }
        std::cout << std::endl;
        return Value(ValType::Int, 0LL);
    } else if (head->nodetype == Type::While) {
        while (evaluate(head->children[0].get(), varmap).boolval) {
            evaluate(head->children[1].get(), varmap);
        }
        return Value(ValType::Int, 0LL);
    } else if (head->nodetype == Type::If) {
        Value ifcondition = evaluate(head->children[0].get(), varmap);
        if (ifcondition.boolval) {
            evaluate(head->children[1].get(), varmap);
        } else if (head->children.size() == 3) {
            evaluate(head->children[2].get(), varmap);
        }
        return Value(ValType::Int, 0LL);
    } else {
        Value num1 = evaluate(head->left.get(), varmap);
        Value num2 = evaluate(head->right.get(), varmap);
        switch (head->nodetype) {
            case Type::Plus:
            if (num1.vtype == ValType::Str && num2.vtype == ValType::Str) {
                return Value(ValType::Str, num1.strval + num2.strval);
            } else if (num1.vtype == ValType::Int && num2.vtype == ValType::Int) {
                return Value(ValType::Int, num1.val + num2.val);
            }
            throw std::runtime_error("Incompatible types for +.\n");
            case Type::Minus:
            return Value(ValType::Int, (num1.val - num2.val));
            case Type::Multiply:
            return Value(ValType::Int, (num1.val * num2.val));
            case Type::Divide:
            if (num2.val == 0) {
                throw std::runtime_error("Cannot divide by zero.\n");
            } else {
                return Value(ValType::Int, (num1.val / num2.val));
            }
            case Type::Exponent:
            if (num2.val < 0) {
                throw std::runtime_error("Cannot have a negative power.\n");
            } else {
                return Value(ValType::Int, static_cast<long long>(std::pow(num1.val, num2.val)));
            }
            case Type::Equality:
            return Value(ValType::Bool, valuecmp(num1, num2) == 0);
            case Type::Less:
            return Value(ValType::Bool, valuecmp(num1, num2) < 0);
            case Type::Greater:
            return Value(ValType::Bool, valuecmp(num1, num2) > 0);
            case Type::LessEqual:
            return Value(ValType::Bool, valuecmp(num1, num2) <= 0);
            case Type::GreaterEqual:
            return Value(ValType::Bool, valuecmp(num1, num2) >= 0);
            case Type::NotEqual:
            return Value(ValType::Bool, valuecmp(num1, num2) != 0);
            default:
            throw std::runtime_error("Invalid nodetype.\n");
        }
    }
}

} // namespace evaluator

} // namespace Geode
