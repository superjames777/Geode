#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cctype>
#include <stdexcept>
#include <cmath>

namespace Geode {

namespace scope {

template <typename T>
class scopestack {
    private:
        std::vector<std::map<std::string, T>> scopes;
    public:
        scopestack() : scopes{std::map<std::string, T>()} {}

        void push() {
            scopes.emplace_back();
            return;
        }

        void pop() {
            if (scopes.size() == 1) {
                throw std::out_of_range("Cannot pop global scope.\n");
            } else {
                scopes.pop_back();
                return;
            }
        }

        void innerdeclare(const std::string &name, const T &val) {
            auto &inner = scopes.back();
            auto found = inner.find(name);
            if (found != inner.end()) {
                throw std::runtime_error("Cannot redeclare a variable.\n");
            } else {
                inner.insert({name, val});
                return;
            }
        }

        T *find(const std::string &name) {
            for (auto iter = scopes.rbegin(); iter != scopes.rend(); iter++) {
                auto &scope = *iter;
                auto found = scope.find(name);
                if (found != scope.end()) {
                    T *val = &found->second;
                    return val;
                }
            }
            return nullptr;
        }

        bool checkinner(const std::string &name) const {
            auto &inner = scopes.back();
            auto found = inner.find(name);
            if (found == inner.end()) {
                return false;
            } else {
                return true;
            }
        }

        size_t depth() const {
            return scopes.size();
        }
};

} // namespace scope

} // namespace Geode
