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

namespace evaluator {

using typenodes::ASTNode;
using typenodes::Type;

double evaluate(ASTNode *head, std::map<std::string, double> &varmap);

double evaluate(ASTNode *head, std::map<std::string, double> &varmap) {
    if (head == nullptr) {
        throw std::runtime_error("Invalid head.\n");
    } else if (head->nodetype == Type::Number) {
        return head->value;
    } else if (head->nodetype == Type::Variable) {
        auto variable = varmap.find(head->name);
        if (variable != varmap.end()) {
            return variable->second;
        }
        throw std::runtime_error("Undefined variable.\n");
    } else if (head->nodetype == Type::Assign) {
        double varval = evaluate(head->left.get(), varmap);
        varmap[head->name] = varval;
        return varval;
    } else if (head->nodetype == Type::Sequence) {
        if (head->children.empty()) {
            throw std::runtime_error("Cannot evaluate an empty sequence.\n");
        }
        double result = 0.0;
        for (const auto &child : head->children) {
            result = evaluate(child.get(), varmap);
        }
        return result;
    } else if (head->nodetype == Type::Print) {
        double toprint = evaluate(head->children[0].get(), varmap);
        std::cout << toprint << std::endl;
        return 0.0;
    } else {
        double num1 = evaluate(head->left.get(), varmap);
        double num2 = evaluate(head->right.get(), varmap);
        switch (head->nodetype) {
            case Type::Plus:
            return num1 + num2;
            case Type::Minus:
            return num1 - num2;
            case Type::Multiply:
            return num1 * num2;
            case Type::Divide:
            if (num2 == 0) {
                throw std::runtime_error("Cannot divide by zero.\n");
            } else {
                return num1 / num2;
            }
            case Type::Exponent:
            return std::pow(num1, num2);
            case Type::Equality:
            return num1 == num2;
            case Type::Less:
            return num1 < num2;
            case Type::Greater:
            return num1 > num2;
            case Type::LessEqual:
            return num1 <= num2;
            case Type::GreaterEqual:
            return num1 >= num2;
            case Type::NotEqual:
            return num1 != num2;
            default:
            throw std::runtime_error("Invalid nodetype.\n");
        }
    }
}

} // namespace evaluator

} // namespace Geode
