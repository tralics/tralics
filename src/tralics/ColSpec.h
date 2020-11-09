#pragma once
#include <string>
#include <vector>

class Xml;

struct ColSpec {
    std::string name;
    std::string model;
    std::string value;
    std::string id;
    Xml *       xval;
    bool        used{false};

    ColSpec(std::string a, std::string b, std::string c);

    auto compare(const std::string &a, const std::string &b) const -> bool { return model == a && value == b; }

    auto get_id() -> std::string { // \todo weird name
        used = true;
        return id;
    }
};

inline std::vector<ColSpec> all_colors;
