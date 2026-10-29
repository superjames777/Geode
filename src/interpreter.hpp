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

Geode::typenodes::Value interpret(const std::string &source, std::map<std::string, Geode::typenodes::Value> &varmap, std::map<std::string, Geode::typenodes::ValType> &typemap);

Geode::typenodes::Value interpret(const std::string &source, std::map<std::string, Geode::typenodes::Value> &varmap, std::map<std::string, Geode::typenodes::ValType> &typemap) {
    std::vector<Geode::typenodes::Token> srctok;
    srctok = Geode::tokenizer::tokenize(source);
    Geode::typenodes::TokCursor srcurs(srctok, 0);
    auto srcnode = Geode::parser::program(srcurs);
    if (srcurs.peek().type != Geode::typenodes::Type::End) {
        throw std::runtime_error("Invalid expression.\n");
    } else {
        std::map<std::string, Geode::typenodes::ValType> typemapcopy(typemap);
        Geode::typechecker::checktype(srcnode.get(), typemapcopy);
        std::map<std::string, Geode::typenodes::Value> varmapcopy(varmap);
        Geode::typenodes::Value result = Geode::evaluator::evaluate(srcnode.get(), varmapcopy);
        varmap = varmapcopy;
        typemap = typemapcopy;
        return result;
    }
}

} // namespace interpreter

} // namespace Geode
