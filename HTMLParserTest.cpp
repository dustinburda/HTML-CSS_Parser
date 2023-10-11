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