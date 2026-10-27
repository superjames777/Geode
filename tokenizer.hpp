#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include "typenodes.hpp"

namespace Geode {

namespace tokenizer {

using typenodes::Token;
using typenodes::Type;
using typenodes::KEYWORDS;

std::vector<Token> tokenize(const std::string &inputstr);

std::vector<Token> tokenize(const std::string &inputstr) {
    std::vector<Token> tokens;
    size_t i = 0;
    while (i < inputstr.size()) {
        if (std::isspace(static_cast<unsigned char>(inputstr[i]))) {
            i++;
        } else if (std::isdigit(static_cast<unsigned char>(inputstr[i]))) {
            size_t startindex = i;
            while (i < inputstr.size() && std::isdigit(static_cast<unsigned char>(inputstr[i]))) {
                i++;
            }
            if (i + 1 < inputstr.size() && inputstr[i] == '.' && std::isdigit(static_cast<unsigned char>(inputstr[i + 1]))) {
                i++;
                while (i < inputstr.size() && std::isdigit(static_cast<unsigned char>(inputstr[i]))) {
                    i++;
                }
            }
            tokens.push_back({ Type::Number, inputstr.substr(startindex, i - startindex) });
        } else if (std::isalpha(static_cast<unsigned char>(inputstr[i])) || inputstr[i] == '_') {
            size_t startindex = i;
            while (i < inputstr.size() && (std::isalnum(static_cast<unsigned char>(inputstr[i])) || inputstr[i] == '_')) {
                i++;
            }
            std::string sub = inputstr.substr(startindex, i - startindex);
            if (KEYWORDS.find(sub) != KEYWORDS.end()) {
                tokens.push_back({ Type::Keyword, sub });
            } else {
                tokens.push_back({ Type::Identifier, sub });
            }
        } else {
            switch (inputstr[i]) {
                case '+':
                tokens.push_back({ Type::Plus, std::string(1, inputstr[i]) });
                break;
                case '-':
                tokens.push_back({ Type::Minus, std::string(1, inputstr[i]) });
                break;
                case '*':
                tokens.push_back({ Type::Multiply, std::string(1, inputstr[i]) });
                break;
                case '/':
                tokens.push_back({ Type::Divide, std::string(1, inputstr[i]) });
                break;
                case '^':
                tokens.push_back({ Type::Exponent, std::string(1, inputstr[i]) });
                break;
                case '(':
                tokens.push_back({ Type::LParentheses, std::string(1, inputstr[i]) });
                break;
                case ')':
                tokens.push_back({ Type::RParentheses, std::string(1, inputstr[i]) });
                break;
                case '=':
                if (i + 1 < inputstr.size() && inputstr[i + 1] == '=') {
                    tokens.push_back({ Type::Equality, inputstr.substr(i, 2) });
                    i++;
                } else {
                    tokens.push_back({ Type::Assignment, std::string(1, inputstr[i]) });
                }
                break;
                case ';':
                tokens.push_back({ Type::Separator, std::string(1, inputstr[i]) });
                break;
                case '<':
                if (i + 1 < inputstr.size() && inputstr[i + 1] == '=') {
                    tokens.push_back({ Type::LessEqual, inputstr.substr(i, 2) });
                    i++;
                } else {
                    tokens.push_back({ Type::Less, std::string(1, inputstr[i]) });
                }
                break;
                case '>':
                if (i + 1 < inputstr.size() && inputstr[i + 1] == '=') {
                    tokens.push_back({ Type::GreaterEqual, inputstr.substr(i, 2) });
                    i++;
                } else {
                    tokens.push_back({ Type::Greater, std::string(1, inputstr[i]) });
                }
                break;
                case '!':
                if (i + 1 < inputstr.size() && inputstr[i + 1] == '=') {
                    tokens.push_back({ Type::NotEqual, inputstr.substr(i, 2) });
                    i++;
                } else {
                    tokens.push_back({ Type::Unknown, std::string(1, inputstr[i]) });
                }
                break;
                default:
                tokens.push_back({ Type::Unknown, std::string(1, inputstr[i]) });
                break;
            }
            i++;
        }
    }
    tokens.push_back({ Type::End, "" });
    return tokens;
}

} // namespace tokenizer

} // namespace Geode
