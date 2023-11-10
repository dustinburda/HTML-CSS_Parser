//
// Created by Dustin Burda on 11/6/23.
//

#ifndef BROWSERENGINEINTERNALS_CSS_H
#define BROWSERENGINEINTERNALS_CSS_H

#include "Declaration.h"
#include "Selector.h"




struct Rule {
    std::vector<Selector> selectors_;
    std::vector<Declaration> declarations_;
};

using Stylesheet = std::vector<Rule>;


#endif //BROWSERENGINEINTERNALS_CSS_H
