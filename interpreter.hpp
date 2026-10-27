#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cctype>
#include <stdexcept>
#include <cmath>

#include "evaluator.hpp"

namespace Geode {

namespace interpreter {

double interpret(const std::string &source, std::map<std::string, double> &varmap);

double interpret(const std::string &source, std::map<std::string, double> &varmap) {
    std::vector<Geode::typenodes::Token> srctok = Geode::tokenizer::tokenize(source);
    Geode::typenodes::TokCursor srcurs(srctok, 0);
    std::unique_ptr<Geode::typenodes::ASTNode> srcnode = nullptr;
    srcnode = Geode::parser::program(srcurs);
    if (srcurs.peek().type != Geode::typenodes::Type::End) {
        throw std::runtime_error("Invalid expression.\n");
    } else {
        double result = Geode::evaluator::evaluate(srcnode.get(), varmap);
        return result;
    }
}

} // namespace interpreter

} // namespace Geode
