//
// Created by Dustin Burda on 11/10/23.
//

#ifndef BROWSERENGINEINTERNALS_DECLARATION_H
#define BROWSERENGINEINTERNALS_DECLARATION_H



#include <vector>
#include <string>
#include <array>
#include <tuple>
#include <variant>
#include <optional>
#include <cstdint>


using Color = std::array<uint8_t, 4>;

enum class Unit {
    Pixel
    // TODO add more
};

using Value = std::variant<std::string, std::tuple<int, Unit>, Color>;

enum class Value_Type {
    Keyword, // string
    Length, // tuple float, Unit
    ColorValue // Color
};

struct Declaration {
    Declaration() = default;

    Declaration(std::string name, std::string value) :
            name_{std::move(name)}, var_value_{std::move(value)}, value_type_{Value_Type::Keyword} {}

    Declaration(std::string name, std::tuple<int, Unit> length) :
            name_{std::move(name)}, var_value_ {length}, value_type_{Value_Type::Length} {}

    Declaration(std::string name, Color color) :
            name_{std::move(name)}, var_value_ {std::move(color)}, value_type_{Value_Type::ColorValue} {}

    std::string name_;
    Value var_value_;
    Value_Type value_type_;
};

int to_px(Declaration& decl) noexcept;




#endif //BROWSERENGINEINTERNALS_DECLARATION_H
