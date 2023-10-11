//
// Created by Dustin Burda on 10/3/23.
//

#ifndef BROWSERENGINEINTERNALS_DOM_H
#define BROWSERENGINEINTERNALS_DOM_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>


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
        explicit Node(std::string text);

        Node(std::string name, AttrMap attr_map, std::vector<node_ptr> children);

        Node_T type() const;

        std::string element_tag_name() const;

        AttrMap element_attributes() const;

        const std::vector<node_ptr>& get_children() const;

        void show(int level) const;

        void show(int level, std::ostringstream& s) const;

    private:
        std::vector<node_ptr> children;
        Node_T node_type;
        std::variant<std::string, ElementData> node_data;
    };
}


#endif //BROWSERENGINEINTERNALS_DOM_H
