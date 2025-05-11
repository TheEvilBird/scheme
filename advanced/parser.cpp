#include "parser.h"
#include "error.h"

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Incorrect input");
    }
    Token token = tokenizer->GetToken();
    tokenizer->Next();
    if (SymbolToken* x = std::get_if<SymbolToken>(&token)) {
        if (x->name == "#t" || x->name == "#f") {
            return std::make_shared<Boolean>(x->name == "#t");
        }
        return std::make_shared<Symbol>(x->name);
    }
    if (QuoteToken* x = std::get_if<QuoteToken>(&token)) {
        // throw SyntaxError("Quote token :(");
        return std::make_shared<Cell>(std::make_shared<Symbol>("quote"),
                                      std::make_shared<Cell>(Read(tokenizer), nullptr));
    }
    if (ConstantToken* x = std::get_if<ConstantToken>(&token)) {
        return std::make_shared<Number>(x->value);
    }
    if (BracketToken* x = std::get_if<BracketToken>(&token)) {
        if (*x == BracketToken::OPEN) {
            return ReadList(tokenizer);
        } else {
            throw SyntaxError("Bracket sequence is not balanced");
        }
    }
    throw SyntaxError("Incorrect token");
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Bracket sequence is not balanced");
    }
    Token token = tokenizer->GetToken();
    if (token == Token{BracketToken::CLOSE}) {
        tokenizer->Next();
        return nullptr;
    }
    std::shared_ptr<Cell> root = std::make_shared<Cell>(nullptr, nullptr);
    std::shared_ptr<Cell> vertex = root;
    std::shared_ptr<Object> left_node = Read(tokenizer);
    root->GetFirst() = left_node;
    token = tokenizer->GetToken();
    while (!tokenizer->IsEnd() && token != Token{DotToken{}} &&
           token != Token{BracketToken::CLOSE}) {
        std::shared_ptr<Object> expr = Read(tokenizer);
        std::shared_ptr<Cell> new_node = std::make_shared<Cell>(expr, nullptr);
        vertex->GetSecond() = new_node;
        vertex = new_node;
        token = tokenizer->GetToken();
    }
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Bracket sequence is not balanced");
    }
    if (token == Token{BracketToken::CLOSE}) {
        tokenizer->Next();
        return root;
    }
    tokenizer->Next();
    std::shared_ptr<Object> right_node = Read(tokenizer);
    vertex->GetSecond() = right_node;
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Bracket sequence is not balanced");
    }
    token = tokenizer->GetToken();
    if (token == Token{BracketToken::CLOSE}) {
        tokenizer->Next();
        return root;
    } else {
        throw SyntaxError("End of list excepted");
    }
}