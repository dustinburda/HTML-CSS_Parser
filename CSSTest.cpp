//
// Created by Dustin Burda on 11/6/23.
//

#include <gtest/gtest.h>
#include "CSS.h"


TEST(CSSTest, SelectorConstructorTest) {
    Selector s;
    EXPECT_EQ(s.tag_name_, std::nullopt);
    EXPECT_EQ(s.id_, std::nullopt);
    EXPECT_EQ(s.class_.size(), 0);
    Specificity s_spec {0,0,0};
    EXPECT_EQ(s.specificity(), s_spec);
    EXPECT_EQ(s.type_, Selector_Type::SimpleSelector);

    Selector h1;
    h1.tag_name_ = "h1";
    h1.class_ = {"note", "blue_box"};
    EXPECT_EQ(h1.tag_name_, "h1");
    EXPECT_EQ(h1.class_.size(), 2);
    EXPECT_EQ(h1.id_, std::nullopt);
    Specificity h1_spec {0, 2, 1};
    EXPECT_EQ(h1.specificity(), h1_spec);
    EXPECT_EQ(h1.type_, Selector_Type::SimpleSelector);

    EXPECT_TRUE(s < h1);
}

TEST(CSSTest, DeclarationConstructorTest) {

}