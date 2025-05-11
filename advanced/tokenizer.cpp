#include "tokenizer.h"

#include <cctype>
#include <iostream>
#include <string>

bool SymbolToken::operator==(const SymbolToken& other) const {
    return name == other.name;
}

bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}

bool DotToken::operator==(const DotToken&) const {
    return true;
}

bool ConstantToken::operator==(const ConstantToken& other) const {
    return value == other.value;
}

Tokenizer::Tokenizer(std::istream* in) : in_(in) {
    Next();
}

bool Tokenizer::IsEnd() {
    return is_end_;
}

void Tokenizer::Next() {
    int nxt = in_->peek();

    while (static_cast<char>(nxt) == ' ' || static_cast<char>(nxt) == '\n') {
        in_->get();
        nxt = in_->peek();
    }

    if (nxt == EOF) {
        is_end_ = true;
        return;
    }

    if (std::isdigit(nxt)) {
        int value;
        *in_ >> value;
        token_ = ConstantToken{value};
        return;
    }

    char c = in_->get();

    if ((c == '+' || c == '-') && std::isdigit(in_->peek())) {
        int sign = (c == '+' ? 1 : -1);
        int value;
        *in_ >> value;
        token_ = ConstantToken{sign * value};
        return;
    }

    switch (c) {
        case '(':
            token_ = BracketToken::OPEN;
            return;
        case ')':
            token_ = BracketToken::CLOSE;
            return;
        case '\'':
            token_ = QuoteToken{};
            return;
        case '.':
            token_ = DotToken{};
            return;
    }

    std::string name;
    name += c;

    if (!CheckSymbolTokenBegin(nxt)) {
        token_ = SymbolToken{name};
        return;
    }

    nxt = in_->peek();
    while (CheckSymbolTokenChar(nxt)) {
        c = in_->get();
        name += c;
        nxt = in_->peek();
    }

    token_ = SymbolToken{name};
}

Token Tokenizer::GetToken() {
    return token_;
}

bool Tokenizer::CheckSymbolTokenBegin(int c) const {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '<' || c == '=' || c == '>' ||
           c == '*' || c == '/' || c == '#';
}

bool Tokenizer::CheckSymbolTokenChar(int c) const {
    return CheckSymbolTokenBegin(c) || ('0' <= c && c <= '9') || c == '?' || c == '!' || c == '-';
}
