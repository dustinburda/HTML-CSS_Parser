//
// Created by Dustin Burda on 11/6/23.
//

#include "CSS.h"

int to_px(Declaration& decl) noexcept {

    int length = 0;
    try{
        auto& value = std::get<std::tuple<int, Unit>>(decl.var_value_);
        length = std::get<0>(value);
    } catch (std::bad_variant_access& e) {
        length = 0;
    }
    return length;
}

Specificity Selector::specificity() const {
    auto a = (id_ == std::nullopt) ? 0 : 1;
    auto b = class_.size();
    auto c = (tag_name_ == std::nullopt) ? 0 : 1;
    return { a, b, c };
}