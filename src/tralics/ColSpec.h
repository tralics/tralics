#pragma once
#include <string>
#include <vector>

class Xml;

class ColSpec {
    std::string name;
    std::string model;
    std::string value;
    std::string id;
    Xml *       xval;
    bool        used;

public:
    ColSpec(std::string a, std::string b, std::string c);

    auto compare(const std::string &a, const std::string &b) -> bool { return model == a && value == b; }
    auto get_id() -> std::string { // \todo weird name
        used = true;
        return id;
    }
    [[nodiscard]] auto is_used() const -> bool { return used; }
    [[nodiscard]] auto get_val() const -> Xml * { return xval; }
};

inline std::vector<ColSpec *> all_colors;
