//
// Created by Dustin Burda on 10/3/23.
//

#include "Dom.h"

using namespace Dom;

Node::Node(std::string text) :children{}, node_type {Node_T::Text} {
    node_data = std::move(text);
}

Node::Node(std::string name, AttrMap attr_map, std::vector<node_ptr> children) : children{std::move(children)}, node_type{Node_T::Element} {
    node_data = ElementData {std::move(name), std::move(attr_map)};
}

Node_T Node::type() const { return node_type; }

std::string Node::element_tag_name() const {
    if(node_type == Node_T::Text)
        return "Text has no tag name";

    auto& element_data = std::get<ElementData>(node_data);

    return element_data.tag_name;
}

AttrMap Node::element_attributes() const {
    if(node_type == Node_T::Text)
        return {};

    auto& element_data = std::get<ElementData>(node_data);

    return element_data.attributes;
}

const std::vector<node_ptr>& Node::get_children() const {
    return children;
}

void Node::show(int level) const {
    if(level != 0)
        std::cout << "\n";

    std::string indent {""};
    indent.resize(level);
    std::fill(indent.begin(), indent.end(), '  ');

    switch (node_type) {
        case Node_T::Text:
            std::cout << indent << std::get<std::string>(node_data);
            break;
        case Node_T::Element:
            auto& element_data = std::get<ElementData>(node_data);
            std::cout << indent << "<" << element_data.tag_name;
            for(const auto& [attr, value] : element_data.attributes) {
                std::cout << " " << attr << "=" << "\"" << value << "\"";
            }
            std::cout << ">";
            break;
    }

    for(const auto& child : children) {
        child->show(level + 2);
    }

    if(node_type == Node_T::Element) {
        auto& element_data = std::get<ElementData>(node_data);
        std::cout << "\n" << indent << "</" << element_data.tag_name << ">";
    }

}

void Node::show(int level, std::ostringstream& s) const{
//    if(level != 0)
//        s << "\n";
//
//    std::string indent {""};
//    indent.resize(level);
//    std::fill(indent.begin(), indent.end(), '  ');
//
//    switch (node_type) {
//        case Node_T::Text:
//            s << indent << std::get<std::string>(node_data);
//            break;
//        case Node_T::Element:
//            auto& element_data = std::get<ElementData>(node_data);
//            s << indent << "<" << element_data.tag_name;
//            for(const auto& [attr, value] : element_data.attributes) {
//                s << " " << attr << "=" << "\"" << value << "\"";
//            }
//            s << ">";
//            break;
//    }
//
//    for(const auto& child : children) {
//        child->show(level + 2);
//    }
//
//    if(node_type == Node_T::Element) {
//        auto& element_data = std::get<ElementData>(node_data);
//        s << "\n" << indent << "</" << element_data.tag_name << ">";
//    }

    if(level != 0)
        s << "\n";

    std::string indent {""};
    indent.resize(level);
    std::fill(indent.begin(), indent.end(), '  ');

    switch (node_type) {
        case Node_T::Text:
            s << indent << std::get<std::string>(node_data);
            break;
        case Node_T::Element:
            auto& element_data = std::get<ElementData>(node_data);
            s << indent << "<" << element_data.tag_name;
            for(const auto& [attr, value] : element_data.attributes) {
                s << " " << attr << "=" << "\"" << value << "\"";
            }
            s << ">";
            break;
    }

    for(const auto& child : children) {
        child->show(level + 2, s);
    }

    if(node_type == Node_T::Element) {
        auto& element_data = std::get<ElementData>(node_data);
        s << "\n" << indent << "</" << element_data.tag_name << ">";
    }

}