//
// Created by Dustin Burda on 11/10/23.
//

#include "Declaration.h"

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