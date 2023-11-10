//
// Created by Dustin Burda on 11/10/23.
//

#include "Selector.h"

Specificity Selector::specificity() const {
    auto a = (id_ == std::nullopt) ? 0 : 1;
    auto b = class_.size();
    auto c = (tag_name_ == std::nullopt) ? 0 : 1;
    return { a, b, c };
}