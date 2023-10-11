//
// Created by Dustin Burda on 10/3/23.
//

#ifndef BROWSERENGINEINTERNALS_DOM_H
#define BROWSERENGINEINTERNALS_DOM_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>


namespace Dom {
    class Node;

    using AttrMap = std::unordered_map<std::string, std::string>;
    using node_ptr = std::shared_ptr<Node>;

    enum class Node_T {
        Text,
        Element
    };

    struct ElementData {
        std::string tag_name;
        AttrMap attributes;
    };


    class Node {
    public:
        explicit Node(std::string text) :children{}, node_type {Node_T::Text} {
            node_data = std::move(text);
        }

        Node(std::string name, AttrMap attr_map, std::vector<node_ptr> children) : children{std::move(children)}, node_type{Node_T::Element} {
            node_data = ElementData {std::move(name), std::move(attr_map)};
        }

        Node_T type() const { return node_type; }

        std::string element_tag_name() const {
            if(node_type == Node_T::Text)
                return "Text has no tag name";

            auto& element_data = std::get<ElementData>(node_data);

            return element_data.tag_name;
        }

        AttrMap element_attributes() const {
            if(node_type == Node_T::Text)
                return {};

            auto& element_data = std::get<ElementData>(node_data);

            return element_data.attributes;
        }

        const std::vector<node_ptr>& get_children() const {
            return children;
        }

        void show(int level = 0) {
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

    private:
        std::vector<node_ptr> children;
        Node_T node_type;
        std::variant<std::string, ElementData> node_data;
    };
}


#endif //BROWSERENGINEINTERNALS_DOM_H
