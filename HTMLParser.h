//
// Created by Dustin Burda on 10/6/23.
//

#ifndef BROWSERENGINEINTERNALS_HTMLPARSER_H
#define BROWSERENGINEINTERNALS_HTMLPARSER_H

#include <string>
#include <cstdlib>

class HTMLParser {
public:
    HTMLParser(std::string& source) : pos{0}, source{std::move(source)} {}

    char peek();
    bool starts_with(std::string& s);
    bool eof();
    char advance();
    std::string advance_while(std::function<bool(char)>& test);

private:
    std::string source;
    size_t pos;
};


#endif //BROWSERENGINEINTERNALS_HTMLPARSER_H
