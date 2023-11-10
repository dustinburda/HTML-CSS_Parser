//
// Created by Dustin Burda on 11/10/23.
//

#ifndef BROWSERENGINEINTERNALS_SELECTOR_H
#define BROWSERENGINEINTERNALS_SELECTOR_H

#include <tuple>
#include <variant>
#include <optional>
#include <string>

using Specificity = std::tuple<uint8_t, uint8_t, uint8_t>;

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

    Specificity specificity() const;

    bool operator<(const Selector& other) const {
        return this->specificity() < other.specificity();
    }
};




#endif //BROWSERENGINEINTERNALS_SELECTOR_H
