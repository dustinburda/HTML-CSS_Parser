//
// Created by Dustin Burda on 10/6/23.
//

#include "HTMLParser.h"

std::optional<char> HTMLParser::peek() const {
    if(eof())
        return std::nullopt;
    return source_[pos_];
}

bool HTMLParser::starts_with(const std::string& s) const {
    if(s.size() > source_.size() - pos_)
        return false;

    //size_t current_pos = pos_;
    // size_t s_pos = 0;

    std::string_view src_view { source_ };
    return src_view.substr(pos_, s.size()) == s;
//    while(current_pos < source_.size() && s_pos < s.size() && source_[current_pos] == s[s_pos]) {
//        if(source_[current_pos] != s[s_pos])
//            return false;
//
//        current_pos++;
//        s_pos++;
//    }

//     return true;
}

bool HTMLParser::eof() const{
    return pos_ >= source_.size();
}

std::optional<char> HTMLParser::advance() {
    if(eof())
        return std::nullopt;
    char ret = source_[pos_];
    pos_++;
    return ret;
}

std::string HTMLParser::advance_while(std::function<bool(char)>& pred) {
    std::string result { "" };

    while(!eof() && pred(peek().value())) {
        result.push_back(advance().value());
    }

    return result;
}

void HTMLParser::advance_whitespace() {
    while(!eof() && (peek() == ' ' || peek() == '\t'))
        advance();
}

std::string HTMLParser::parse_tag_name() {
    std::function<bool(char)> is_alphanum = [](char elem) -> bool {
        return ( elem >= 'a' && elem <= 'z' ) || ( elem >= 'A' && elem <= 'Z' ) || ( elem >= '0' && elem <= '9');
    };
    return advance_while(is_alphanum);
}

Dom::node_ptr HTMLParser::parse_node() {
    switch(peek().value()) {
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
