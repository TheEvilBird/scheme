#include "scheme.h"
#include "error.h"
#include "commands.h"
#include <exception>
#include <unordered_map>
#include <functional>
#include <string>

Interpreter::Interpreter() : global_(std::make_shared<Scope>(kCommands)) {
    //    for (auto &[name, f] : kCommands) {
    //        global_->SetVariable(name, f);
    //    }
}

std::string Interpreter::Run(const std::string& str) {
    std::stringstream ss{str};
    Tokenizer tokenizer{&ss};
    auto ast = Read(&tokenizer);
    if (!tokenizer.IsEnd()) {
        throw SyntaxError("Input is incorrect");
    }
    if (ast == nullptr) {
        return "()";
    }
    auto result = ast->Eval(global_);
    if (result == nullptr) {
        return "()";
    }
    return result->Serialize();
}
