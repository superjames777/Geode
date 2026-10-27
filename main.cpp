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
comparison
expression
term
unary
exponent
factor

*/

int main(int argc, char *argv[]) {
    std::map<std::string, double> varmap;
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
            Geode::interpreter::interpret(script, varmap);
        } catch (const std::exception &e) {
            std::cerr << e.what();
            return 1;
        }
    } else {
        time_t timestamp;
        time(&timestamp);
        std::cout << "Geode v0.4.0 - " << ctime(&timestamp) << std::endl;
        std::string equation;
        std::cout << "> ";
        while (std::getline(std::cin, equation)) {
            if (equation == "quit" || equation == "QUIT") {
                break;
            } else if (equation.empty()) {
                std::cout << "> ";
                continue;
            }
            double result = 0.0;
            try {
                result = Geode::interpreter::interpret(equation, varmap);
                std::cout << result << std::endl;
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
