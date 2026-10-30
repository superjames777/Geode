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

Geode::typenodes::Value interpret(const std::string &source, Geode::scope::scopestack<Geode::typenodes::Value> &varmap, Geode::scope::scopestack<Geode::typenodes::ValType> &scopemap);

Geode::typenodes::Value interpret(const std::string &source, Geode::scope::scopestack<Geode::typenodes::Value> &varmap, Geode::scope::scopestack<Geode::typenodes::ValType> &scopemap) {
    std::vector<Geode::typenodes::Token> srctok;
    srctok = Geode::tokenizer::tokenize(source);
    Geode::typenodes::TokCursor srcurs(srctok, 0);
    auto srcnode = Geode::parser::program(srcurs);
    if (srcurs.peek().type != Geode::typenodes::Type::End) {
        throw std::runtime_error("Invalid expression.\n");
    } else {
        Geode::scope::scopestack<Geode::typenodes::ValType> typemapcopy(scopemap);
        Geode::typechecker::checktype(srcnode.get(), typemapcopy);
        Geode::scope::scopestack<Geode::typenodes::Value> varmapcopy(varmap);
        Geode::typenodes::Value result = Geode::evaluator::evaluate(srcnode.get(), varmapcopy);
        varmap = varmapcopy;
        scopemap = typemapcopy;
        return result;
    }
}

} // namespace interpreter

} // namespace Geode
