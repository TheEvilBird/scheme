#pragma once

#include "tokenizer.h"
#include "parser.h"
#include "object.h"
#include "error.h"
#include <exception>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>

class Function : public Object {
public:
    virtual ~Function() = default;
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args) {
        throw std::runtime_error("Not implemented");
    }
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope) {
        throw std::runtime_error("Can not eval function");
    }
    virtual std::string Serialize() {
        return "function";
    }
};

class OperatorPlus : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class OperatorMinus : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class OperatorMult : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class OperatorDiv : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class ComparisonEqual : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class ComparisonLess : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class ComparisonGreater : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class ComparisonLessEq : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class ComparisonGreaterEq : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class FunctorMax : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class FunctorMin : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class FunctorAbs : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class IntegerPredicate : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class BooleanPredicate : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class PairPredicate : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class NullPredicate : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class ListPredicate : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class SymbolPredicate : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class LogicNot : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class LogicAnd : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class LogicOr : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class IfConditional : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class QuoteCommand : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class OperationCons : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class OperationCar : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class OperationCdr : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class OperationList : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class OperationListRef : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class OperationListTail : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class DefineCommand : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class SetCommand : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class SetCarCommand : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class SetCdrCommand : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class CreateLambda : public Function {
public:
    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);
};

class LambdaCallback : public Function {
public:
    LambdaCallback(const std::shared_ptr<Scope>& par,
                   const std::vector<std::shared_ptr<Object>>& args,
                   const std::vector<std::shared_ptr<Object>>& body);

    virtual std::shared_ptr<Object> operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args);

private:
    std::shared_ptr<Scope> par_scope_ = nullptr;
    std::vector<std::shared_ptr<Object>> argv_;
    std::vector<std::shared_ptr<Object>> body_;
};

class AbstactCommand : public Function {};

static const std::unordered_map<std::string, std::shared_ptr<Object>> kCommands = {
    {"+", std::make_shared<OperatorPlus>()},
    {"-", std::make_shared<OperatorMinus>()},
    {"*", std::make_shared<OperatorMult>()},
    {"/", std::make_shared<OperatorDiv>()},

    {"=", std::make_shared<ComparisonEqual>()},
    {"<", std::make_shared<ComparisonLess>()},
    {">", std::make_shared<ComparisonGreater>()},
    {"<=", std::make_shared<ComparisonLessEq>()},
    {">=", std::make_shared<ComparisonGreaterEq>()},

    {"max", std::make_shared<FunctorMax>()},
    {"min", std::make_shared<FunctorMin>()},
    {"abs", std::make_shared<FunctorAbs>()},

    {"number?", std::make_shared<IntegerPredicate>()},
    {"boolean?", std::make_shared<BooleanPredicate>()},
    {"pair?", std::make_shared<PairPredicate>()},
    {"null?", std::make_shared<NullPredicate>()},
    {"list?", std::make_shared<ListPredicate>()},

    {"symbol?", std::make_shared<SymbolPredicate>()},

    {"not", std::make_shared<LogicNot>()},
    {"and", std::make_shared<LogicAnd>()},
    {"or", std::make_shared<LogicOr>()},

    {"cons", std::make_shared<OperationCons>()},
    {"car", std::make_shared<OperationCar>()},
    {"cdr", std::make_shared<OperationCdr>()},

    {"list", std::make_shared<OperationList>()},
    {"list-ref", std::make_shared<OperationListRef>()},
    {"list-tail", std::make_shared<OperationListTail>()},

    {"quote", std::make_shared<QuoteCommand>()},

    {"define", std::make_shared<DefineCommand>()},
    {"set!", std::make_shared<SetCommand>()},
    {"if", std::make_shared<IfConditional>()},
    {"set-car!", std::make_shared<SetCarCommand>()},
    {"set-cdr!", std::make_shared<SetCdrCommand>()},
    {"lambda", std::make_shared<CreateLambda>()},
    // {"", std::make_shared<AbstactCommand>()}

};