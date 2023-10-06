//
// Created by Dustin Burda on 10/3/23.
//

#ifndef BROWSERENGINEINTERNALS_DOM_H
#define BROWSERENGINEINTERNALS_DOM_H

#include <vector>
#include <string>
#include <memory>


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

        Node(std::string name, const AttrMap& attr_map, const std::vector<node_ptr>& children) : children{std::move(children)}, node_type{Node_T::Element} {
            node_data = ElementData {std::move(name), std::move(attr_map)};
        }

    private:
        std::vector<node_ptr> children;
        Node_T node_type;
        std::variant<std::string, ElementData> node_data;
    };
}


#endif //BROWSERENGINEINTERNALS_DOM_H
