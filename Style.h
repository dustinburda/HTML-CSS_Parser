//
// Created by Dustin Burda on 11/10/23.
//

#ifndef BROWSERENGINEINTERNALS_STYLE_H
#define BROWSERENGINEINTERNALS_STYLE_H

#include <unordered_map>
#include <string>
#include "Dom.h"
#include "CSS.h"

struct StyleNode;

using PropertyMap = std::unordered_map<std::string, Value>;
using style_node_ptr = std::shared_ptr<StyleNode>;

struct StyleNode {
    Dom::node_ptr node;
    PropertyMap specified_values;
    std::vector<style_node_ptr> children;
};

static bool matches_simple_selector(Dom::ElementData& elem, Selector& selector) {
    if(selector.tag_name_.has_value() && selector.tag_name_.value() != elem.tag_name) {
        return false;
    }

    if(selector.id_.has_value() && elem.id().has_value() && selector.id_.value() == elem.id().value()) {
        return false;
    }

    for(auto& class_ : selector.class_) {
        if(elem.classes().count(class_) > 0) {
            return false;
        }
    }

    return true;
}

static bool matches(Dom::ElementData& elem, Selector& selector) {
    // TODO add more selectors
    switch (selector.type_) {
        case Selector_Type::SimpleSelector:
            return matches_simple_selector(elem, selector);
            break;
        default:
            break;
    }
}

using MatchedRule = std::pair<Specificity, Rule>;

static std::optional<MatchedRule> match_rule(Dom::ElementData& elem, Rule& rule) {
    auto match_it = std::find_if(rule.selectors_.begin(), rule.selectors_.end(), [&elem](auto& selector) {
        return matches(elem, selector);
    });

    auto rule_specificity = match_it->specificity();
    return std::make_pair(rule_specificity, rule );
}

static std::vector<MatchedRule> matching_rules(Dom::ElementData& elem, Stylesheet& stylesheet) {
    std::vector<MatchedRule> matched_rules;

    for(auto& rule : stylesheet) {
        auto matched_rule = match_rule(elem, rule);

        if(matched_rule != std::nullopt)
            matched_rules.push_back(matched_rule.value());
    }

    return matched_rules;
}










#endif //BROWSERENGINEINTERNALS_STYLE_H
