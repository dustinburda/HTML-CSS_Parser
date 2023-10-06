//
// Created by Dustin Burda on 10/6/23.
//

#ifndef BROWSERENGINEINTERNALS_HTMLPARSER_H
#define BROWSERENGINEINTERNALS_HTMLPARSER_H

#include <string>
#include <cstdlib>
#include "Dom.h"

using ss_pair = std::pair<std::string, std::string>;

class HTMLParser {
public:
    explicit HTMLParser(const std::string& source) : pos{0}, source{std::move(source)} {}

    Dom::node_ptr parse(std::string& source);


private:
    char peek() const;
    bool starts_with(const std::string& s) const;
    bool eof() const;

    char advance();
    std::string advance_while(std::function<bool(char)>& test);
    void advance_whitespace();


    std::string parse_tag_name();
    Dom::node_ptr parse_node();
    Dom::node_ptr parse_text();
    Dom::node_ptr parse_element();

    ss_pair parse_attr();
    std::string parse_attr_value();
    Dom::AttrMap parse_attributes();

    std::vector<Dom::node_ptr> parse_nodes();


    std::string source;
    size_t pos;
};


#endif //BROWSERENGINEINTERNALS_HTMLPARSER_H
