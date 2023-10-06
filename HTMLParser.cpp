//
// Created by Dustin Burda on 10/6/23.
//

#include "HTMLParser.h"

char HTMLParser::peek() const {
    return source[pos];
}

bool HTMLParser::starts_with(const std::string& s) const {
    size_t current_pos = pos;
    size_t s_pos = 0;

    while(current_pos < source.size() && s_pos < s.size() && source[current_pos] == s[s_pos]) {
        if(source[current_pos] != s[s_pos])
            return false;

        current_pos++;
        s_pos++;
    }

    return true;
}

bool HTMLParser::eof() const{
    return pos >= source.size();
}

char HTMLParser::advance() {
    char ret = source[pos];
    pos++;
    return ret;
}

std::string HTMLParser::advance_while(std::function<bool(char)>& test) {
    std::string result { "" };

    while(!eof() && test(peek())) {
        result.push_back(advance());
    }

    return result;
}

void HTMLParser::advance_whitespace() {
    while(!eof() && peek() == ' ')
        advance();
}

std::string HTMLParser::parse_tag_name() {
    std::function<bool(char)> is_alphanum = [](char elem) -> bool {
        return ( elem >= 'a' && elem <= 'z' ) || ( elem >= 'A' && elem <= 'Z' ) || ( elem >= '0' && elem <= '9');
    };
    return advance_while(is_alphanum);
}

Dom::node_ptr HTMLParser::parse_node() {
    switch(advance()) {
        case '<':
            return parse_element();
        default:
            return parse_text();
    }
}

Dom::node_ptr HTMLParser::parse_text() {
    std::function<bool(char)> not_open_tag = [](char elem) -> bool {
        return elem != '<';
    };

    std::string text = std::move(advance_while(not_open_tag));
    std::shared_ptr<Dom::Node> n_ptr = std::make_shared<Dom::Node>(std::move(text));

    return n_ptr;
}

Dom::node_ptr HTMLParser::parse_element() {
    auto tag_name = parse_tag_name();
    auto attributes = parse_attributes();
    auto children = parse_nodes();

    std::shared_ptr<Dom::Node> n_ptr = std::make_shared<Dom::Node>(tag_name, attributes, children);

    return n_ptr;
}

ss_pair HTMLParser::parse_attr() {
    return {parse_tag_name(), parse_attr_value()};
}


std::string HTMLParser::parse_attr_value() {
    auto open_quote = advance();

    std::function<bool(char)> not_open_quote = [&open_quote](char elem) -> bool {
        return elem != open_quote;
    };

    auto value = advance_while(not_open_quote);

    return value;
}

Dom::AttrMap HTMLParser::parse_attributes() {
    Dom::AttrMap attr_map{};
    while(true) {
        advance_whitespace();
        if( peek() == '>' ) {
            break;
        }

        auto [name, value] = parse_attr();
        attr_map[name] = value;
    }

    return attr_map;
}

std::vector<Dom::node_ptr> HTMLParser::parse_nodes() {
    std::vector<Dom::node_ptr> nodes {};
    while(true) {
        advance_whitespace();

        if(eof() || starts_with("</")) {
            break;
        }
        nodes.push_back(parse_node());
    }

    return nodes;
}

Dom::node_ptr HTMLParser::parse(std::string& source) {
    auto nodes = parse_nodes();

    if(nodes.size() == 1) {
       return nodes[0];
    } else {
        return std::make_shared<Dom::Node>("html", Dom::AttrMap {}, nodes);
    }
}
