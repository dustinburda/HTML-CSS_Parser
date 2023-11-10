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
    // margin: auto
    Declaration d1 {"margin", "auto"};
    EXPECT_EQ(d1.value_type_, Value_Type::Keyword);
    EXPECT_EQ(d1.name_, "margin");
    EXPECT_EQ(std::get<std::string>(d1.var_value_), "auto");
    EXPECT_EQ(to_px(d1), 0);

    // margin-bottom: 10px
    Declaration d2 {"margin-bottom", {10, Unit::Pixel}};
    EXPECT_EQ(d2.value_type_, Value_Type::Length);
    EXPECT_EQ(d2.name_, "margin-bottom");
    auto true_val2 = std::get<std::tuple<int, Unit>>(d2.var_value_);
    std::tuple<int, Unit> expected_val2 = std::tuple<int, Unit>(10, Unit::Pixel);
    EXPECT_EQ(true_val2, expected_val2);
    EXPECT_EQ(to_px(d2), 10);

    // color: FF0000
    Declaration d3 {"color", Color {255, 0, 0, 255} };
    EXPECT_EQ(d3.value_type_, Value_Type::ColorValue);
    EXPECT_EQ(d3.name_, "color");
    auto true_val3 = std::get<Color>(d3.var_value_);
    Color expected_val3 = Color {255, 0, 0, 255};
    EXPECT_EQ(true_val3, expected_val3);
    EXPECT_EQ(to_px(d3), 0);
}