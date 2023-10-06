//
// Created by Dustin Burda on 10/6/23.
//

#include "HTMLParser.h"

char HTMLParser::peek() {
    return source[pos];
}

bool HTMLParser::starts_with(std::string& s) {
    int current_pos = pos;
    int s_pos = 0;

    while(current_pos < source.size() && s_pos < s.size() && source[current_pos] == s[s_pos]) {
        if(source[current_pos] != s[s_pos])
            return false;

        current_pos++;
        s_pos++;
    }

    return true;
}

bool HTMLParser::eof() {
    return pos >= source.size();
}

char HTMLParser::advance() {
    char ret = source[pos];
    pos++;
    return ret;
}

std::string HTMLParser::advance_while(std::function<bool(char)>& test) {
    std::string result { "" };

    while(!eof() && test(peek())) {
        result.push_back(advance());
    }

    return result;
}