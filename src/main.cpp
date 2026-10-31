#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <ctime>

#include "interpreter.hpp"

// For debugging:
static void preorder(Geode::typenodes::ASTNode *head);

/* 
The levels:

program
statement
block
comparison
expression
term
unary
exponent
factor

*/

int main(int argc, char *argv[]) {
    Geode::scope::scopestack<Geode::typenodes::Value> varmap;
    Geode::scope::scopestack<Geode::typenodes::ValType> scopemap;
    if (argc == 2) {
        std::ifstream geofile(argv[1]);
        if (!geofile.is_open()) {
            std::cerr << "Failed to open file " << argv[1] << "." << std::endl;
            return 1;
        }
        std::string script(
            std::istreambuf_iterator<char>(geofile),
            std::istreambuf_iterator<char>{}
        );
        try {
            Geode::interpreter::interpret(script, varmap, scopemap);
        } catch (const std::exception &e) {
            std::cerr << e.what();
            return 1;
        }
    } else {
        time_t timestamp;
        time(&timestamp);
        std::cout << "Geode v1.6.0 - " << ctime(&timestamp) << std::endl;
        std::string equation;
        std::cout << "> ";
        while (std::getline(std::cin, equation)) {
            if (equation == "quit" || equation == "QUIT") {
                break;
            } else if (equation.empty()) {
                std::cout << "> ";
                continue;
            }
            try {
                Geode::typenodes::Value result = Geode::interpreter::interpret(equation, varmap, scopemap);
                switch (result.vtype) {
                    case Geode::typenodes::ValType::Int:
                    std::cout << result.val;
                    break;
                    case Geode::typenodes::ValType::Str:
                    std::cout << result.strval;
                    break;
                    case Geode::typenodes::ValType::Bool:
                    std::cout << (result.boolval ? "true" : "false");
                    break;
                }
                std::cout << std::endl;
            } catch (const std::exception &e) {
                std::cerr << e.what();
            }
            std::cout << "> ";
        }
    }
    return 0;
}

static void preorder(Geode::typenodes::ASTNode *head) {
    if (head != nullptr) {
        std::cout << Geode::typenodes::TYPESTRS[static_cast<int>(head->nodetype)] << " " << head->value << std::endl;
        preorder(head->left.get());
        preorder(head->right.get());
    }
    return;
}
