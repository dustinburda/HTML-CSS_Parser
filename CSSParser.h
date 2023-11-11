//
// Created by Dustin Burda on 11/6/23.
//

#ifndef BROWSERENGINEINTERNALS_CSSPARSER_H
#define BROWSERENGINEINTERNALS_CSSPARSER_H

#include "CSS.h"

static std::function<bool(char)> valid_identifier = [](char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '-') || (c == '_');
};


class CSSParser {
public:
    explicit CSSParser(std::string source) : source_{std::move(source)}, pos_{0} {}

    std::string advance_while(std::function<bool(char)>& pred) {
        std::string result { "" };

        while(!eof() && pred(peek().value())) {
            result.push_back(advance().value());
        }

        return result;
    }

    void advance_whitespace() {
        while(!eof() && (peek() == ' ' || peek() == '\t' || peek() == '\n'))
            advance();
    }

    std::optional<char> advance() {
        if(eof())
            return std::nullopt;
        char ret = source_[pos_];
        pos_++;
        return ret;
    }

    std::optional<std::string> advance(size_t n) {
        size_t advance_amount = std::min(n, source_.size() - pos_);

        if(advance_amount == 0)
            return std::nullopt;

        std::string ret = source_.substr(pos_, advance_amount);
        pos_ += advance_amount;

        return ret;
    }

    bool eof() const{
        return pos_ >= source_.size();
    }

    std::optional<char> peek() const {
        if(eof())
            return std::nullopt;
        return source_[pos_];
    }

    std::vector<Rule> parse() {
        Stylesheet rules;

        while(true) {
            advance_whitespace();
            if(eof())
                break;

            rules.push_back(parse_rule());
        }

        return rules;
    }


    Rule parse_rule() {
        std::vector<Selector> selectors = std::move(parse_selectors());
        std::vector<Declaration> declarations = std::move(parse_declarations());

        return { selectors, declarations };
    }

    std::vector<Selector> parse_selectors() {
        std::vector<Selector> ret;
        while(true) {
            ret.emplace_back(parse_selector());
            advance_whitespace();
            if(peek() == ',') {
                advance(); // ','
                advance_whitespace();
            } else if (peek() == '{') {
                break;
            } else {
                throw std::logic_error("Unexpected character!");
            }
        }
        std::sort(ret.begin(), ret.end());

        return ret;
    }

    Selector parse_selector() {
        Selector selector;

        while(!eof()) {
            if(peek() == '#') {
                advance();
                selector.id_ = parse_identifier();
            } else if (peek() == '.') {
                advance();
                selector.class_.emplace_back(parse_identifier());
            } else if (peek() == '*') {
                advance();
            } else if(valid_identifier(peek().value())) {
                selector.tag_name_ = parse_identifier();
            } else {
                break;
            }
        }

        return selector;
    }

    std::string parse_identifier() {
        return advance_while(valid_identifier);
    }

    std::vector<Declaration> parse_declarations() {
        advance(); // '{'
        std::vector<Declaration> ret;
        while(true) {
            advance_whitespace();
            if(peek() == '}') {
                advance();
                break;
            }
            ret.emplace_back(parse_declaration());
        }
        return ret;
    }

    Declaration parse_declaration() {
        Declaration declaration;

        declaration.name_ = parse_identifier();
        advance_whitespace();
        advance(); // ':'
        advance_whitespace();

        if(std::isdigit(peek().value())) {
            declaration.var_value_ = parse_length();
            declaration.value_type_ = Value_Type::Length;
        } else if(peek().value() == '#') {
            declaration.var_value_ = parse_color();
            declaration.value_type_ = Value_Type::ColorValue;
        } else {
            declaration.var_value_ = parse_identifier();
            declaration.value_type_ = Value_Type::Keyword;
        }

        advance_whitespace();
        advance(); // ';'


        return declaration;
    }

    std::tuple<int, Unit> parse_length() {
        std::function<bool(char)> num_or_dot = [](char c) {
            return (c >= '0' && c <= '9') || (c == '.');
        };
        auto length_str = advance_while(num_or_dot);
        float length = std::stoi(length_str);

        auto unit_type = parse_identifier(); // 'px'
        if(unit_type != "px")
            throw std::logic_error("Not valid unit!");

        Unit unit_t = Unit::Pixel;

        return {length, unit_t};
    }

    Color parse_color() {
        advance(); // '#'
        auto r = std::stoi(advance(2).value(), nullptr, 16);
        auto g = std::stoi(advance(2).value(), nullptr, 16);
        auto b = std::stoi(advance(2).value(), nullptr, 16);

        return Color {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), 255};
    }

private:
    std::string source_;
    size_t pos_;
};

#endif //BROWSERENGINEINTERNALS_CSSPARSER_H
