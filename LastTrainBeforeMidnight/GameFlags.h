#pragma once
#include <string>
#include <unordered_map>

struct GameFlags
{
    std::unordered_map<std::string, bool> f;

    bool get(const std::string& k) const {
        auto it = f.find(k);
        return (it != f.end() && it->second);
    }

    void set(const std::string& k, bool v = true) {
        f[k] = v;
    }
};

// global
extern GameFlags gFlags;
