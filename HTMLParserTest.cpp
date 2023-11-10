//
// Created by Dustin Burda on 10/10/23.
//

#include <gtest/gtest.h>
#include "HTMLParser.h"

TEST(HTMLParser, BasicTest) {
    HTMLParser p {"Hello World!"};
    EXPECT_EQ(p.peek(), 'H');
    EXPECT_EQ(p.advance(), 'H');
    EXPECT_FALSE(p.eof());
    EXPECT_TRUE(p.starts_with("ello"));

    EXPECT_FALSE(p.starts_with("llo"));
    EXPECT_EQ(p.advance(), 'e');
    EXPECT_FALSE(p.eof());
    EXPECT_TRUE(p.starts_with("llo"));
    EXPECT_TRUE(p.starts_with("llo Wor"));

    while(!p.eof()) {
        p.advance();
    }
    EXPECT_EQ(p.peek(), std::nullopt);
    EXPECT_EQ(p.advance(), std::nullopt);
}

TEST(HTMLParser, ParseTextTest) {
    HTMLParser p {"Hello"};

    auto n_ptr = p.parse_text();
    std::ostringstream s1;
    n_ptr->show(0, s1);

    auto n_ptr_true = std::make_shared<Dom::Node>("Hello");
    std::ostringstream s2;
    n_ptr_true->show(0, s2);
    EXPECT_EQ(s1.str(), s2.str());
}

TEST(HTMLParser, AdvanceAndTagNameTest) {
    HTMLParser p {"     Hello      P\t  \nQ  \n\tr87V\nZaB123"};

    p.advance_whitespace();

    EXPECT_EQ(p.peek(), 'H');
    EXPECT_TRUE(p.starts_with("Hell"));

    auto hello_text = p.parse_tag_name();
    EXPECT_EQ(hello_text, "Hello");

    p.advance_whitespace();

    auto p_text = p.parse_tag_name();
    EXPECT_EQ(p_text, "P");

    p.advance_whitespace();

    auto q_text = p.parse_tag_name();
    EXPECT_EQ(q_text, "Q");

    p.advance_whitespace();

    auto r87v_text = p.parse_tag_name();
    EXPECT_EQ(r87v_text, "r87V");

    p.advance_whitespace();

    auto zab123_text = p.parse_tag_name();
    EXPECT_EQ(zab123_text, "ZaB123");
}

TEST(HTMLParser, ParseTextAndParseNode_Text) {
    std::string html_src1 = "     Hello      P\t  \nQ  \n\tr87V\nZaB123";
    HTMLParser p1 {html_src1};

    auto text_ptr1 = p1.parse_node();
    std::ostringstream s1;
    text_ptr1->show(0, s1);
    EXPECT_EQ(s1.str(), html_src1);
    std::cout << s1.str();


    HTMLParser p2 {"     Hello      P\t < \nQ  \n\tr87V\nZaB123"};

    auto text_ptr2 = p2.parse_node();
    std::ostringstream s2;
    text_ptr2->show(0, s2);

    EXPECT_EQ(s2.str(), "     Hello      P\t ");
    std::cout << s2.str();
}

TEST(HTMLParser, ParseElementAndParseNode_Element) {
    std::string html_src1 = "<h1 class=\"Hello\" id=\"World\"></h1>";
    HTMLParser p1 { html_src1 };

    auto element_ptr = p1.parse_node();
    std::ostringstream s1;
    element_ptr->show(0, s1);
    std::cout << s1.str();
    EXPECT_EQ(element_ptr->element_attributes().size(), 2);
    EXPECT_EQ(element_ptr->element_attributes()["class"], "Hello");
    EXPECT_EQ(element_ptr->element_attributes()["id"], "World");
}

TEST(HTMLParser, ParseNodeFull) {
  std::string html_src1 = "<h1 class=\"Hello\" id=\"World\">\n"
                          "  Hello World!"
                          "</h1>";
    HTMLParser p1 { html_src1 };
    auto html_ptr1 = p1.parse();
    EXPECT_EQ(html_ptr1->element_attributes().size(), 2);
    EXPECT_EQ(html_ptr1->element_attributes()["class"], "Hello");
    EXPECT_EQ(html_ptr1->element_attributes()["id"], "World");
    EXPECT_EQ(html_ptr1->get_children().size(), 1);
    EXPECT_EQ(html_ptr1->get_children()[0]->type(), Dom::Node_T::Text);
    html_ptr1->show(0);

    std::cout << "\n\n";

    std::string html_src2 = "<body>\n"
                            "  <h1>Title</h1>\n"
                            "    <div id=\"main\" class=\"test\">\n"
                            "      <p>Hello <em>world</em>!</p>\n"
                            "  </div>\n"
                            "</body>\n"
                            "<h2 class=\"Text\" id=\"Big\">\n"
                            "  Hey Hey Hey!"
                            "</h2>\n";
    HTMLParser p2 { html_src2 };
    auto html_ptr2 = p2.parse();
    EXPECT_EQ(html_ptr2->get_children().size(), 2);

    html_ptr2->show(0);
}