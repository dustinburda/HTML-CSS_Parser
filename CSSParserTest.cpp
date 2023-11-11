//
// Created by Dustin Burda on 11/6/23.
//

#include <gtest/gtest.h>

#include "CSSParser.h"

TEST(CSSParserTest, ParseColorTest) {
   std::string CSS_Src1 = "#FF0000";
   CSSParser p1{ CSS_Src1 };

   auto color1 = p1.parse_color();
   Color expected_color1 {255, 0, 0, 255};
   EXPECT_EQ(color1, expected_color1);

    std::string CSS_Src2 = "#A21FC3";
    CSSParser p2 {CSS_Src2};
    auto color2 = p2.parse_color();
    Color expected_color2 { 0xA2, 0x1F, 0xC3, 255 };
    EXPECT_EQ(expected_color2, color2);
}

TEST(CSSParserTest, ParseLengthTest) {
    std::string CSS_Src1 = "20px";
    CSSParser p1 { CSS_Src1 };

    auto [amount1, unit1] = p1.parse_length();
    EXPECT_EQ(amount1, 20);
    EXPECT_EQ(unit1, Unit::Pixel);


    std::string CSS_Src2 ="553px";
    CSSParser p2 { CSS_Src2 };

    auto [amount2, unit2] = p2.parse_length();
    EXPECT_EQ(amount2, 553);
    EXPECT_EQ(unit2, Unit::Pixel);
}

TEST(CSSParser, ParseDeclarationTest) {
    std::string CSS_Src1 = "margin-bottom: 20px";
    CSSParser p1 { CSS_Src1 };
    auto decl1 = p1.parse_declaration();
    EXPECT_EQ(decl1.value_type_, Value_Type::Length);
    EXPECT_EQ(decl1.name_, "margin-bottom");
    std::tuple<int, Unit> actual_length = std::get<std::tuple<int, Unit>>(decl1.var_value_);
    std::tuple<int, Unit> expected_length = {20, Unit::Pixel};
    EXPECT_EQ(actual_length, expected_length);

    std::string CSS_Src2 = "color: #CC0000";
    CSSParser p2 { CSS_Src2 };
    auto decl2 = p2.parse_declaration();
    EXPECT_EQ(decl2.value_type_,Value_Type::ColorValue);
    EXPECT_EQ(decl2.name_, "color");
    auto actual_color = std::get<Color>(decl2.var_value_);
    auto expected_color = Color { 0xCC, 0x0, 0x0, 0xFF };
    EXPECT_EQ(actual_color, expected_color);

    std::string CSS_Src3 = "margin: auto";
    CSSParser p3 { CSS_Src3 };
    auto decl3 = p3.parse_declaration();
    EXPECT_EQ(decl3.value_type_, Value_Type::Keyword);
    EXPECT_EQ(decl3.name_, "margin");
    auto actual_keyword = std::get<std::string>(decl3.var_value_);
    auto expected_keyword = "auto";
    EXPECT_EQ(actual_keyword, expected_keyword);
}

TEST(CSSParser, ParseDeclarations) {
    std::string CSS_Src1 = "{"
                          "margin-bottom: 20px;\n"
                          "color: #CC0000;\n"
                          "margin: auto;\n"
                          "}";

    CSSParser p1 { CSS_Src1 };
    auto declarations = p1.parse_declarations();
    EXPECT_EQ(declarations.size(), 3);

    EXPECT_EQ(declarations[0].value_type_, Value_Type::Length);
    EXPECT_EQ(declarations[0].name_, "margin-bottom");
    std::tuple<int, Unit> actual_length = std::get<std::tuple<int, Unit>>(declarations[0].var_value_);
    std::tuple<int, Unit> expected_length = {20, Unit::Pixel};
    EXPECT_EQ(actual_length, expected_length);

    EXPECT_EQ(declarations[1].value_type_, Value_Type::ColorValue);
    EXPECT_EQ(declarations[1].name_, "color");
    Color actual_color = std::get<Color>(declarations[1].var_value_);
    Color expected_color = {0xCC, 0x0, 0x0, 0xFF};
    EXPECT_EQ(actual_color, expected_color);


    EXPECT_EQ(declarations[2].value_type_, Value_Type::Keyword);
    EXPECT_EQ(declarations[2].name_, "margin");
    std::string actual_keyword = std::get<std::string>(declarations[2].var_value_);
    std::string expected_keyword = "auto";
    EXPECT_EQ(actual_keyword, expected_keyword);
}

TEST(CSSParser, ParseSelector) {
    std::string CSS_Src1 = "div.note.text";
    CSSParser p1 { CSS_Src1 };

    auto selector1 = p1.parse_selector();
    EXPECT_EQ(selector1.class_.size(), 2);
    EXPECT_EQ(selector1.type_, Selector_Type::SimpleSelector);
    EXPECT_EQ(selector1.id_, std::nullopt);
    EXPECT_EQ(selector1.tag_name_.value(), "div");


    std::string CSS_Src2 = "#hello";
    CSSParser p2 { CSS_Src2 };

    auto selector2 = p2.parse_selector();
    EXPECT_EQ(selector2.id_, "hello");
    EXPECT_EQ(selector2.tag_name_, std::nullopt);
    EXPECT_EQ(selector2.class_.size(), 0);

    std::string CSS_Src3 = "h1";
    CSSParser p3 { CSS_Src3 };

    auto selector3 = p3.parse_selector();
    EXPECT_EQ(selector3.tag_name_.value(), "h1");
    EXPECT_EQ(selector3.id_, std::nullopt);
    EXPECT_EQ(selector3.class_.size(), 0);
}

TEST(CSSParser, ParseSelectors) {
    std::string CSS_Src1 = "h1, h2, h3 {";
    CSSParser p1 { CSS_Src1 };

    auto selectors1 = p1.parse_selectors();
    EXPECT_EQ(selectors1.size(), 3);
    EXPECT_EQ(selectors1[0].tag_name_, "h1");
    EXPECT_EQ(selectors1[1].tag_name_, "h2");
    EXPECT_EQ(selectors1[2].tag_name_, "h3");

    std::string CSS_Src2 = "h1, #answer, div.note {";
    CSSParser p2 { CSS_Src2 };

    auto selectors2 = p2.parse_selectors();
    EXPECT_EQ(selectors2.size(), 3);

    EXPECT_EQ(selectors2[0].id_, std::nullopt);
    EXPECT_EQ(selectors2[0].class_.size(), 0);
    EXPECT_EQ(selectors2[0].tag_name_.value(), "h1");

    EXPECT_EQ(selectors2[1].tag_name_, "div");
    EXPECT_EQ(selectors2[1].class_.size(), 1);
    EXPECT_EQ((selectors2[1].class_)[0], "note");

    EXPECT_EQ(selectors2[2].id_.value(), "answer");
    EXPECT_EQ(selectors2[2].tag_name_, std::nullopt);
    EXPECT_EQ(selectors2[2].class_.size(), 0);
}

TEST(CSSParser, ParseRule) {
    std::string CSS_Src1 = "h1, body.row, #column { "
                           "margin-top: auto; "
                           "color: #FFA21C; "
                           "}";
    CSSParser p1 { CSS_Src1 };
    auto rule = p1.parse_rule();
    EXPECT_EQ(rule.selectors_.size(), 3);
    EXPECT_EQ(rule.selectors_[0].tag_name_, "h1");
    EXPECT_EQ(rule.selectors_[1].tag_name_, "body");
    EXPECT_EQ(rule.selectors_[1].class_.size(), 1);
    EXPECT_EQ(rule.selectors_[1].class_[0], "row");
    EXPECT_EQ(rule.selectors_[2].id_, "column");
    EXPECT_EQ(rule.declarations_.size(), 2);
    EXPECT_EQ(rule.declarations_[0].name_, "margin-top");
    EXPECT_EQ(rule.declarations_[0].value_type_, Value_Type::Keyword);
    auto actual_keyword = std::get<std::string>(rule.declarations_[0].var_value_);
    auto expected_keyword = "auto";
    EXPECT_EQ(actual_keyword, expected_keyword);
    EXPECT_EQ(rule.declarations_[1].name_, "color");
    EXPECT_EQ(rule.declarations_[1].value_type_, Value_Type::ColorValue);
    auto actual_color = std::get<Color>(rule.declarations_[1].var_value_);
    Color expected_color = Color { 0xFF, 0xA2, 0x1C, 0xFF};
    EXPECT_EQ(actual_color, expected_color);
}

TEST(CSSParser, ParseStylesheet) {
    std::string CSS_Src1 = "h1, body.row, #column { "
                           "margin-top: auto; "
                           "color: #FFA21C; "
                           "}"
                           "#thisid { display: flex; }"
                           "body.column { background-color: #FFEA2C; }";
    CSSParser p1 { CSS_Src1 };
    Stylesheet s1 = p1.parse();
    EXPECT_EQ(s1.size(), 3);
}

