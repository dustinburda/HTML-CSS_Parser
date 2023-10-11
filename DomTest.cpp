//
// Created by Dustin Burda on 10/10/23.
//

#include <gtest/gtest.h>
#include "Dom.h"

TEST(DOM, ConstructorTest) {
    Dom::node_ptr t1 = std::make_shared<Dom::Node>("Hello");
    EXPECT_EQ(t1->type(), Dom::Node_T::Text);
    Dom::node_ptr t2 = std::make_shared<Dom::Node>("world");
    EXPECT_EQ(t2->type(), Dom::Node_T::Text);
    Dom::node_ptr t3 = std::make_shared<Dom::Node>("!");
    EXPECT_EQ(t3->type(), Dom::Node_T::Text);

    Dom::AttrMap attr_map1 {};
    std::vector<Dom::node_ptr> child1 {t2};
    Dom::node_ptr e1 = std::make_shared<Dom::Node>("em", attr_map1, child1);
    EXPECT_EQ(e1->type(), Dom::Node_T::Element);
    EXPECT_EQ(e1->element_tag_name(), "em");
    EXPECT_TRUE(e1->element_attributes().empty());
    EXPECT_EQ(e1->get_children().size(), 1);
    EXPECT_EQ(e1->get_children()[0], t2);


    Dom::AttrMap attr_map2 {};
    std::vector<Dom::node_ptr> child2 {t1, e1, t3};
    Dom::node_ptr p = std::make_shared<Dom::Node>("p", attr_map2, child2);
    EXPECT_EQ(p->type(), Dom::Node_T::Element);
    EXPECT_EQ(p->element_tag_name(), "p");
    EXPECT_TRUE(p->element_attributes().empty());
    EXPECT_EQ(p->get_children().size(), 3);
    EXPECT_EQ(p->get_children()[0], t1);
    EXPECT_EQ(p->get_children()[1], e1);
    EXPECT_EQ(p->get_children()[2], t3);



    Dom::AttrMap attr_map3 {{"id", "main"}, {"class", "test"}};
    std::vector<Dom::node_ptr> child3 {p};
    Dom::node_ptr div = std::make_shared<Dom::Node>("div", attr_map3, child3);
    EXPECT_EQ(div->type(), Dom::Node_T::Element);
    EXPECT_EQ(div->element_tag_name(), "div");
    EXPECT_EQ(div->element_attributes().size(), 2);
    EXPECT_EQ(div->element_attributes()["id"], "main");
    EXPECT_EQ(div->element_attributes()["class"], "test");
    EXPECT_EQ(div->get_children().size(), 1);
    EXPECT_EQ(div->get_children()[0], p);


    div->show(0);
}


