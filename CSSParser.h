//
// Created by Dustin Burda on 11/6/23.
//

#ifndef BROWSERENGINEINTERNALS_CSSPARSER_H
#define BROWSERENGINEINTERNALS_CSSPARSER_H

#include "CSS.h"

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

    bool eof() const{
        return pos_ >= source_.size();
    }

    std::optional<char> peek() const {
        if(eof())
            return std::nullopt;
        return source_[pos_];
    }

    std::vector<Rule> parse_rules() {
        std::vector<Rule> rules;

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
        std::vector<Declaration> declarations; // = std::move(parse_declarations());

        return { selectors, declarations };
    }

    std::vector<Selector> parse_selectors() {
        std::vector<Selector> ret;
        while(true) {
            ret.emplace_back(parse_selector());
            advance_whitespace();
            if(peek() == ',') {
                advance();
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

//        while(!eof()) {
//            if(peek() == '#') {
//                advance();
//                selector.id_ = parse_identifier();
//            } else if (peek() == '.') {
//                advance();
//                selector.class_.emplace_back((parse_identifier()));
//            } else if (peek() == '*') {
//                advance();
//            } else if(valid_identifier(peek())) {
//                selector.tag_name_ = parse_identifier();
//            } else {
//                break;
//            }
//        }

        return selector;
    }

private:
    std::string source_;
    size_t pos_;
};


#endif //BROWSERENGINEINTERNALS_CSSPARSER_H
