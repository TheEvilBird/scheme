#include "object.h"
#include "error.h"
#include "scheme.h"
#include "commands.h"

#include <exception>

// Number =========================================================================================

Number::Number(int64_t value) : value_(value) {
}

int64_t Number::GetValue() const {
    return value_;
}
std::shared_ptr<Object> Number::Eval(std::shared_ptr<Scope> scope) {
    return shared_from_this();
}
std::string Number::Serialize() {
    return std::to_string(GetValue());
}

// Symbol =========================================================================================

Symbol::Symbol(const std::string& name) : name_(name) {
}

const std::string& Symbol::GetName() const {
    return name_;
}

std::shared_ptr<Object> Symbol::Eval(std::shared_ptr<Scope> scope) {
    //    return std::make_shared<Symbol>(GetName());
    //    auto var_scope = scope->FindScopeWithVariable(GetName());
    //    return As<Scope>(var_scope)->GetVariable(GetName());
    return scope->FindVariable(GetName());
}

std::string Symbol::Serialize() {
    return GetName();
}

// Bool =========================================================================================

Boolean::Boolean(bool flag) : Symbol(flag ? "#t" : "#f") {
}

std::shared_ptr<Object> Boolean::Eval(std::shared_ptr<Scope> scope) {
    return shared_from_this();
}

// Cell ==========================================================================================

Cell::Cell(std::shared_ptr<Object> f, std::shared_ptr<Object> s) : first_(f), second_(s) {
}

std::shared_ptr<Object> Cell::GetFirst() const {
    return first_;
}

std::shared_ptr<Object> Cell::GetSecond() const {
    return second_;
}

std::shared_ptr<Object>& Cell::GetFirst() {
    return first_;
}

std::shared_ptr<Object>& Cell::GetSecond() {
    return second_;
}
std::shared_ptr<Object> Cell::Eval(std::shared_ptr<Scope> scope) {
    if (GetFirst() == nullptr) {
        throw RuntimeError("Nothing to eval");
    }
    auto cmd = GetFirst()->Eval(scope);
    //    if (!Is<Symbol>(GetFirst())) {
    //        throw RuntimeError("Nothing to eval");
    //    }
    //    auto cmd = As<Symbol>(GetFirst())->Eval(scope);
    if (!Is<Function>(cmd)) {
        throw RuntimeError("Wrong command");
    }
    //    auto args = GetArgs(second_);
    return (*As<Function>(cmd))(scope, GetSecond());
}

std::string Cell::Serialize() {
    std::string result = "(";
    if (GetFirst() == nullptr) {
        result += "()";
    } else {
        result += GetFirst()->Serialize();
    }
    auto vertex = GetSecond();
    while (Is<Cell>(vertex)) {
        result += " " + As<Cell>(vertex)->GetFirst()->Serialize();
        vertex = As<Cell>(vertex)->GetSecond();
    }
    if (vertex != nullptr) {
        result += " . " + vertex->Serialize();
    }
    result += ')';
    return result;
}

// Scope ==========================================================================================

Scope::Scope(const std::shared_ptr<Scope>& par) : par_(par) {
}

Scope::Scope(const std::unordered_map<std::string, std::shared_ptr<Object>>& vars)
    : par_(nullptr), vars_(vars) {
}

Scope::Scope(const std::shared_ptr<Scope>& par,
             const std::unordered_map<std::string, std::shared_ptr<Object>>& vars)
    : par_(par), vars_(vars) {
}

std::shared_ptr<Object>& Scope::FindVariable(const std::string& name) {
    //    if (kCommands.find(name) != kCommands.end()) {
    if (vars_.find(name) != vars_.end()) {
        //        return kCommands.at(name);
        return vars_[name];
    }
    if (par_ != nullptr) {
        return par_->FindVariable(name);
    }
    throw NameError("No such name");
}

std::shared_ptr<Object> Scope::FindScopeWithVariable(const std::string& name) {
    //    if (kCommands.find(name) != kCommands.end()) {
    if (vars_.find(name) != vars_.end()) {
        //        return kCommands.at(name);
        //        return std::make_shared<Scope>(*this);
        return shared_from_this();
    }
    if (par_ != nullptr) {
        return par_->FindScopeWithVariable(name);
    }
    throw NameError("No such name");
}

std::shared_ptr<Object> Scope::Eval(std::shared_ptr<Scope> scope) {
    throw RuntimeError("todo eval scope");
}

std::string Scope::Serialize() {
    throw RuntimeError("todo eval serialize");
}

void Scope::SetVariable(const std::string& name, std::shared_ptr<Object> val) {
    vars_[name] = val;
}

std::shared_ptr<Object> Scope::GetVariable(const std::string& name) {
    return vars_[name];
}

// std::unordered_map<std::string, std::shared_ptr<Object>> Scope::GetAllVars() {
//     return vars_;
// }
//
// void Scope::SetAllVars(const std::unordered_map<std::string, std::shared_ptr<Object>> &vars) {
//     vars_ = vars;
// }

// Lambda ==========================================================================================

// Lambda::Lambda(const std::shared_ptr<Scope>& par, const std::vector<std::shared_ptr<Object>>&
// args,
//                const std::vector<std::shared_ptr<Object>>& body)
//     : par_(par), args_(args), body_(body) {
// }
