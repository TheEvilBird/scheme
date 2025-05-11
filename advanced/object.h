#pragma once

#include <memory>
#include <execution>
#include <string>
#include <unordered_map>

class Scope;

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope) = 0;
    virtual std::string Serialize() = 0;
};

class Number : public Object {
public:
    Number(int64_t value);
    int64_t GetValue() const;
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope);
    virtual std::string Serialize();

private:
    int64_t value_ = 0;
};

class Symbol : public Object {
public:
    Symbol(const std::string& name);
    const std::string& GetName() const;
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope);
    virtual std::string Serialize();

private:
    std::string name_;
};

class Boolean : public Symbol {
public:
    Boolean(bool flag);
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope);
};

class Cell : public Object {
public:
    Cell() = default;
    Cell(std::shared_ptr<Object> f, std::shared_ptr<Object> s);
    std::shared_ptr<Object> GetFirst() const;
    std::shared_ptr<Object> GetSecond() const;
    std::shared_ptr<Object>& GetFirst();
    std::shared_ptr<Object>& GetSecond();
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope);
    virtual std::string Serialize();

private:
    std::shared_ptr<Object> first_ = nullptr;
    std::shared_ptr<Object> second_ = nullptr;
};

class Scope : public Object {
public:
    Scope() = default;
    Scope(const std::shared_ptr<Scope>& par);
    Scope(const std::unordered_map<std::string, std::shared_ptr<Object>>& vars);
    Scope(const std::shared_ptr<Scope>& par,
          const std::unordered_map<std::string, std::shared_ptr<Object>>& vars);
    std::shared_ptr<Object>& FindVariable(const std::string& name);
    std::shared_ptr<Object> FindScopeWithVariable(const std::string& name);
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope);
    virtual std::string Serialize();
    void SetVariable(const std::string& name, std::shared_ptr<Object> val);
    std::shared_ptr<Object> GetVariable(const std::string& name);
    //    std::unordered_map<std::string, std::shared_ptr<Object>> GetAllVars();
    //    void SetAllVars(const std::unordered_map<std::string, std::shared_ptr<Object>>& vars);

private:
    std::shared_ptr<Scope> par_ = nullptr;
    std::unordered_map<std::string, std::shared_ptr<Object>> vars_;
};

// class Lambda : public Object {
// public:
//     Lambda() = default;
//     Lambda(const std::shared_ptr<Scope>& scope, const std::vector<std::shared_ptr<Object>>& argv,
//            const std::vector<std::shared_ptr<Object>>& body);
//
// private:
//     std::shared_ptr<Scope> par_ = nullptr;
//     std::vector<std::shared_ptr<Object>> args_;
//     std::vector<std::shared_ptr<Object>> body_;
// };

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and conversion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    auto cast_res = dynamic_pointer_cast<T>(obj);
    //    if (cast_res == nullptr) {
    //        throw std::invalid_argument("No conversion");
    //    }
    return cast_res;
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    auto cast_res = dynamic_pointer_cast<T>(obj);
    if (cast_res == nullptr) {
        return false;
    }
    return true;
}

std::vector<std::shared_ptr<Object>> GetArgs(std::shared_ptr<Object> obj);