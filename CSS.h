//
// Created by Dustin Burda on 11/6/23.
//

#ifndef BROWSERENGINEINTERNALS_CSS_H
#define BROWSERENGINEINTERNALS_CSS_H


#include <vector>
#include <string>
#include <array>
#include <tuple>
#include <variant>
#include <optional>
#include <cstdint>

using Specificty = std::tuple<uint8_t, uint8_t, uint8_t>;

enum Selector_Type {
    SimpleSelector
    // TODO add more
};

struct Selector {
    Selector() : type_{Selector_Type::SimpleSelector}, tag_name_{std::nullopt}, id_{std::nullopt}, class_{} {}

    Selector_Type type_;
    std::optional<std::string> tag_name_;
    std::optional<std::string> id_;
    std::vector<std::string> class_;

    Specificty specificity() const {
        auto a = (id_ == std::nullopt) ? 0 : 1;
        auto b = class_.size();
        auto c = (tag_name_ == std::nullopt) ? 0 : 1;
        return { a, b, c };
    }

    bool operator<(const Selector& other) const {
        return this->specificity() < other.specificity();
    }
};


using Color = std::array<uint8_t, 4>;

enum class Unit {
    Pixel
    // TODO add more
};

enum class Value {
    Keyword, // string
    Length, // tuple float, Unit
    ColorValue // Color
};

struct Declaration {
    Declaration() = default;

    Declaration(std::string name, std::string value) :
        name_{std::move(name)}, value_{std::move(value)}, value_type_{Value::Keyword} {}

    Declaration(std::string name, std::tuple<float, Unit> length) :
        name_{std::move(name)}, value_ {length}, value_type_{Value::Length} {}

    Declaration(std::string name, Color color) :
        name_{std::move(name)}, value_ {std::move(color)}, value_type_{Value::ColorValue} {}

    std::string name_;
    std::variant<std::string, std::tuple<float, Unit>, Color> value_;
    Value value_type_;
};

static float to_px(Declaration& decl) {

    float length = 0;
    try{
        auto& value = std::get<std::tuple<float, Unit>>(decl.value_);
        length = std::get<0>(value);
    } catch (std::bad_variant_access) {
        length = 0;
    }
    return length;
}


struct Rule {
    std::vector<Selector> selectors_;
    std::vector<Declaration> declarations_;
};

using Stylesheet = std::vector<Rule>;


#endif //BROWSERENGINEINTERNALS_CSS_H
