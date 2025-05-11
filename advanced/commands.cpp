#include "commands.h"
#include "object.h"
#include "error.h"

#include <cassert>

bool IsFalse(std::shared_ptr<Object> obj) {
    if (Is<Symbol>(obj) && As<Symbol>(obj)->GetName() == "#f") {
        return true;
    }
    return false;
}

bool IsTrue(std::shared_ptr<Object> obj) {
    return !IsFalse(obj);
}

size_t CountArgs(std::shared_ptr<Object> argv) {
    size_t argc = 0;
    auto arg = argv;
    while (arg != nullptr && Is<Cell>(arg)) {
        ++argc;
        arg = As<Cell>(arg)->GetSecond();
    }
    if (arg != nullptr) {
        ++argc;
    }
    return argc;
}

std::vector<std::shared_ptr<Object>> GetArgs(std::shared_ptr<Object> obj) {
    std::vector<std::shared_ptr<Object>> args;
    auto ptr = As<Cell>(obj);
    while (ptr != nullptr) {
        args.emplace_back(ptr->GetFirst());
        ptr = As<Cell>(ptr->GetSecond());
    }
    return args;
}

std::shared_ptr<Object> OperatorPlus::operator()(std::shared_ptr<Scope> scope,
                                                 std::shared_ptr<Object> args) {
    int64_t result = 0;
    auto arg = As<Cell>(args);

    while (arg != nullptr) {
        auto res = arg->GetFirst();
        if (res == nullptr) {
            throw RuntimeError("A number expected in addition");
        }
        res = res->Eval(scope);
        if (!Is<Number>(res)) {
            throw RuntimeError("A number expected in addition");
        }
        result += As<Number>(res)->GetValue();
        arg = As<Cell>(arg->GetSecond());
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> OperatorMinus::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    int64_t result = 0;
    auto arg = As<Cell>(args);

    if (arg == nullptr && !Is<Number>(arg)) {
        throw RuntimeError("Non-empty sequence expected in subtraction");
    }
    if (Is<Number>(arg)) {
        result = As<Number>(arg)->GetValue();
    } else {
        bool first = true;
        while (arg != nullptr) {
            auto res = arg->GetFirst();
            if (res == nullptr) {
                throw RuntimeError("A number expected in subtraction");
            }
            res = res->Eval(scope);
            if (!Is<Number>(res)) {
                throw RuntimeError("A number expected in subtraction");
            }
            if (first) {
                result = As<Number>(res)->GetValue();
                first = false;
            } else {
                result -= As<Number>(res)->GetValue();
            }
            arg = As<Cell>(arg->GetSecond());
        }
    }

    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> OperatorMult::operator()(std::shared_ptr<Scope> scope,
                                                 std::shared_ptr<Object> args) {
    int64_t result = 1;
    auto arg = As<Cell>(args);

    while (arg != nullptr) {
        auto res = arg->GetFirst();
        if (res == nullptr) {
            throw RuntimeError("A number expected in multiplication");
        }
        res = res->Eval(scope);
        if (!Is<Number>(res)) {
            throw RuntimeError("A number expected in multiplication");
        }
        result *= As<Number>(res)->GetValue();
        arg = As<Cell>(arg->GetSecond());
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> OperatorDiv::operator()(std::shared_ptr<Scope> scope,
                                                std::shared_ptr<Object> args) {
    int64_t result = 0;
    auto arg = As<Cell>(args);

    if (arg == nullptr && !Is<Number>(arg)) {
        throw RuntimeError("Non-empty sequence expected in division");
    }
    if (Is<Number>(arg)) {
        result = As<Number>(arg)->GetValue();
    } else {
        bool first = true;
        while (arg != nullptr) {
            auto res = arg->GetFirst();
            if (res == nullptr) {
                throw RuntimeError("A number expected in division");
            }
            res = res->Eval(scope);
            if (!Is<Number>(res)) {
                throw RuntimeError("A number expected in division");
            }
            if (first) {
                result = As<Number>(res)->GetValue();
                first = false;
            } else {
                result /= As<Number>(res)->GetValue();
            }
            arg = As<Cell>(arg->GetSecond());
        }
    }

    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> ComparisonEqual::operator()(std::shared_ptr<Scope> scope,
                                                    std::shared_ptr<Object> args) {
    if (args == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    auto arg = As<Cell>(args);

    int64_t value = 0;
    bool first = true;
    do {
        auto res = arg->GetFirst();
        if (res == nullptr) {
            throw RuntimeError("A number expected in equal comparison");
        }
        res = res->Eval(scope);
        if (!Is<Number>(res)) {
            throw RuntimeError("Not a number in equal comparison");
        }
        if (first) {
            value = As<Number>(res)->GetValue();
            first = false;
        } else {
            if (value != As<Number>(res)->GetValue()) {
                return std::make_shared<Boolean>(false);
            }
        }
        arg = As<Cell>(arg->GetSecond());
    } while (arg != nullptr);
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> ComparisonLess::operator()(std::shared_ptr<Scope> scope,
                                                   std::shared_ptr<Object> args) {
    if (args == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    auto arg = As<Cell>(args);

    int64_t value = 0;
    bool first = true;
    do {
        auto res = arg->GetFirst();
        if (res == nullptr) {
            throw RuntimeError("A number expected in less comparison");
        }
        res = res->Eval(scope);
        if (!Is<Number>(res)) {
            throw RuntimeError("Not a number in less comparison");
        }
        if (first) {
            value = As<Number>(res)->GetValue();
            first = false;
        } else {
            if (!(value < As<Number>(res)->GetValue())) {
                return std::make_shared<Boolean>(false);
            }
        }
        arg = As<Cell>(arg->GetSecond());
    } while (arg != nullptr);
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> ComparisonGreater::operator()(std::shared_ptr<Scope> scope,
                                                      std::shared_ptr<Object> args) {
    if (args == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    auto arg = As<Cell>(args);

    int64_t value = 0;
    bool first = true;
    do {
        auto res = arg->GetFirst();
        if (res == nullptr) {
            throw RuntimeError("A number expected in greater comparison");
        }
        res = res->Eval(scope);
        if (!Is<Number>(res)) {
            throw RuntimeError("Not a number in greater comparison");
        }
        if (first) {
            value = As<Number>(res)->GetValue();
            first = false;
        } else {
            if (!(value > As<Number>(res)->GetValue())) {
                return std::make_shared<Boolean>(false);
            }
        }
        arg = As<Cell>(arg->GetSecond());
    } while (arg != nullptr);
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> ComparisonLessEq::operator()(std::shared_ptr<Scope> scope,
                                                     std::shared_ptr<Object> args) {
    if (args == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    auto arg = As<Cell>(args);

    int64_t value = 0;
    bool first = true;
    do {
        auto res = arg->GetFirst();
        if (res == nullptr) {
            throw RuntimeError("A number expected in less-eq comparison");
        }
        res = res->Eval(scope);
        if (!Is<Number>(res)) {
            throw RuntimeError("Not a number in less-eq comparison");
        }
        if (first) {
            value = As<Number>(res)->GetValue();
            first = false;
        } else {
            if (!(value <= As<Number>(res)->GetValue())) {
                return std::make_shared<Boolean>(false);
            }
        }
        arg = As<Cell>(arg->GetSecond());
    } while (arg != nullptr);
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> ComparisonGreaterEq::operator()(std::shared_ptr<Scope> scope,
                                                        std::shared_ptr<Object> args) {
    if (args == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    auto arg = As<Cell>(args);

    int64_t value = 0;
    bool first = true;
    do {
        auto res = arg->GetFirst();
        if (res == nullptr) {
            throw RuntimeError("A number expected in greater-eq comparison");
        }
        res = res->Eval(scope);
        if (!Is<Number>(res)) {
            throw RuntimeError("Not a number in greater-eq comparison");
        }
        if (first) {
            value = As<Number>(res)->GetValue();
            first = false;
        } else {
            if (!(value >= As<Number>(res)->GetValue())) {
                return std::make_shared<Boolean>(false);
            }
        }
        arg = As<Cell>(arg->GetSecond());
    } while (arg != nullptr);
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> FunctorMax::operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args) {
    if (CountArgs(args) == 0) {
        throw RuntimeError("Non-empty sequence expected in max functor");
    }

    int64_t result = INT64_MIN;
    auto arg = As<Cell>(args);

    do {
        auto res = arg->GetFirst();
        if (res == nullptr) {
            throw RuntimeError("A number expected in max functor");
        }
        res = res->Eval(scope);
        if (!Is<Number>(res)) {
            throw RuntimeError("A number expected in max functor");
        }
        result = std::max(result, As<Number>(res)->GetValue());
        arg = As<Cell>(arg->GetSecond());
    } while (arg != nullptr);
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> FunctorMin::operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args) {
    if (CountArgs(args) == 0) {
        throw RuntimeError("Non-empty sequence expected in min functor");
    }

    int64_t result = INT64_MAX;
    auto arg = As<Cell>(args);

    do {
        auto res = arg->GetFirst();
        if (res == nullptr) {
            throw RuntimeError("A number expected in min functor");
        }
        res = res->Eval(scope);
        if (!Is<Number>(res)) {
            throw RuntimeError("A number expected in min functor");
        }
        result = std::min(result, As<Number>(res)->GetValue());
        arg = As<Cell>(arg->GetSecond());
    } while (arg != nullptr);

    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> FunctorAbs::operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in abs command");
    }

    auto arg = As<Cell>(args)->GetFirst();
    if (arg == nullptr) {
        throw RuntimeError("Not a number in abs");
    }
    arg = arg->Eval(scope);
    if (!Is<Number>(arg)) {
        throw RuntimeError("Not a number in abs");
    }

    int64_t result = As<Number>(arg)->GetValue();
    if (result < 0) {
        result = -result;
    }

    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> IntegerPredicate::operator()(std::shared_ptr<Scope> scope,
                                                     std::shared_ptr<Object> args) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in integer predicate");
    }
    auto arg = As<Cell>(args)->GetFirst();
    if (arg == nullptr) {
        std::make_shared<Boolean>(false);
    }
    arg = arg->Eval(scope);
    return std::make_shared<Boolean>(Is<Number>(arg));
}

std::shared_ptr<Object> BooleanPredicate::operator()(std::shared_ptr<Scope> scope,
                                                     std::shared_ptr<Object> args) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in boolean predicate");
    }
    auto arg = As<Cell>(args)->GetFirst();
    if (arg == nullptr) {
        std::make_shared<Boolean>(false);
    }
    arg = arg->Eval(scope);

    if (Is<Symbol>(arg) &&
        (As<Symbol>(arg)->GetName() == "#t" || As<Symbol>(arg)->GetName() == "#f")) {
        return std::make_shared<Boolean>(true);
    }
    return std::make_shared<Boolean>(false);
}

std::shared_ptr<Object> PairPredicate::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in pair predicate");
    }
    auto arg = As<Cell>(args)->GetFirst();
    if (arg == nullptr) {
        std::make_shared<Boolean>(false);
    }
    arg = arg->Eval(scope);

    if (Is<Cell>(arg)) {
        return std::make_shared<Boolean>(true);
    }
    return std::make_shared<Boolean>(false);
}

std::shared_ptr<Object> NullPredicate::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in null predicate");
    }
    auto arg = As<Cell>(args)->GetFirst();
    if (arg == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    arg = arg->Eval(scope);
    if (arg == nullptr) {
        return std::make_shared<Boolean>(true);
    }

    if (Is<Cell>(arg) && As<Cell>(arg)->GetFirst() == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    return std::make_shared<Boolean>(false);
}

std::shared_ptr<Object> ListPredicate::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in list predicate");
    }
    auto arg = As<Cell>(args)->GetFirst();
    if (arg == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    arg = arg->Eval(scope);
    if (arg == nullptr) {
        return std::make_shared<Boolean>(true);
    }

    if (Is<Cell>(arg)) {
        while (arg != nullptr && Is<Cell>(arg)) {
            arg = As<Cell>(arg)->GetSecond();
        }

        if (arg == nullptr) {
            return std::make_shared<Boolean>(true);
        }
        return std::make_shared<Boolean>(false);
    }
    return std::make_shared<Boolean>(false);
}

std::shared_ptr<Object> SymbolPredicate::operator()(std::shared_ptr<Scope> scope,
                                                    std::shared_ptr<Object> args) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in integer predicate");
    }
    auto arg = As<Cell>(args)->GetFirst();
    if (arg == nullptr) {
        std::make_shared<Boolean>(false);
    }
    arg = arg->Eval(scope);
    return std::make_shared<Boolean>(Is<Symbol>(arg));
}

std::shared_ptr<Object> LogicNot::operator()(std::shared_ptr<Scope> scope,
                                             std::shared_ptr<Object> args) {
    //    if (args == nullptr || As<Cell>(args)->GetSecond() != nullptr) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in logic not");
    }
    auto arg = As<Cell>(args)->GetFirst();
    if (arg == nullptr) {
        std::make_shared<Boolean>(false);
    }
    arg = arg->Eval(scope);

    if (IsFalse(arg)) {
        return std::make_shared<Boolean>(true);
    }
    return std::make_shared<Boolean>(false);
}

std::shared_ptr<Object> LogicAnd::operator()(std::shared_ptr<Scope> scope,
                                             std::shared_ptr<Object> args) {
    //    if (args == nullptr) {
    //        std::make_shared<Boolean>(true);
    //    }
    auto arg = As<Cell>(args);

    std::shared_ptr<Object> last = std::make_shared<Boolean>(true);
    while (arg != nullptr) {
        auto res = arg->GetFirst();
        if (res != nullptr) {
            res = res->Eval(scope);
            if (IsFalse(res)) {
                return res;
            }
        }
        last = res;
        arg = As<Cell>(arg->GetSecond());
    }
    return last;
}

std::shared_ptr<Object> LogicOr::operator()(std::shared_ptr<Scope> scope,
                                            std::shared_ptr<Object> args) {
    //    if (args == nullptr) {
    //        std::make_shared<Boolean>(false);
    //    }
    auto arg = As<Cell>(args);

    std::shared_ptr<Object> last = std::make_shared<Boolean>(false);
    while (arg != nullptr) {
        auto res = arg->GetFirst();
        if (res != nullptr) {
            res = res->Eval(scope);
            if (IsTrue(res)) {
                return res;
            }
        }
        last = res;
        arg = As<Cell>(arg->GetSecond());
    }
    return last;
}

std::shared_ptr<Object> QuoteCommand::operator()(std::shared_ptr<Scope> scope,
                                                 std::shared_ptr<Object> args) {
    if (args == nullptr || As<Cell>(args)->GetSecond() != nullptr) {
        throw RuntimeError("An argument expected in quote");
    }
    return As<Cell>(args)->GetFirst();
}

std::shared_ptr<Object> OperationCons::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    if (CountArgs(args) != 2) {
        throw RuntimeError("Two arguments expected in cons operation");
    }

    auto first = As<Cell>(args)->GetFirst();
    if (first != nullptr) {
        first = first->Eval(scope);
    }
    auto second = As<Cell>(args)->GetSecond();
    if (second != nullptr) {
        second = As<Cell>(second)->GetFirst();
        if (second != nullptr) {
            second = second->Eval(scope);
        }
    }

    return std::make_shared<Cell>(first, second);
}

std::shared_ptr<Object> OperationCar::operator()(std::shared_ptr<Scope> scope,
                                                 std::shared_ptr<Object> args) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in car operation");
    }

    auto arg = As<Cell>(args)->GetFirst();
    if (arg != nullptr) {
        arg = arg->Eval(scope);
    }
    if (arg == nullptr) {
        throw RuntimeError("null result in car operation");
    }
    if (!Is<Cell>(arg)) {
        arg = arg->Eval(scope);
    }
    auto first = As<Cell>(arg)->GetFirst();
    return first;
}

std::shared_ptr<Object> OperationCdr::operator()(std::shared_ptr<Scope> scope,
                                                 std::shared_ptr<Object> args) {
    if (CountArgs(args) != 1) {
        throw RuntimeError("One argument expected in cdr operation");
    }

    auto arg = As<Cell>(args)->GetFirst();
    //    auto prev = arg;
    //    prev = nullptr;
    if (arg != nullptr) {
        arg = arg->Eval(scope);
    }
    if (arg == nullptr) {
        throw RuntimeError("null result in car operation");
    }
    if (!Is<Cell>(arg)) {
        arg = arg->Eval(scope);
    }
    auto second = As<Cell>(arg)->GetSecond();
    return second;
}

std::shared_ptr<Object> OperationList::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    return args;
}

std::shared_ptr<Object> OperationListRef::operator()(std::shared_ptr<Scope> scope,
                                                     std::shared_ptr<Object> args) {
    if (CountArgs(args) != 2) {
        throw RuntimeError("Two arguments expected in list_ref operation");
    }

    auto first = As<Cell>(args)->GetFirst();
    if (first != nullptr) {
        first = first->Eval(scope);
    }
    auto second = As<Cell>(args)->GetSecond();
    if (second != nullptr) {
        second = As<Cell>(second)->GetFirst();
        if (second != nullptr) {
            second = second->Eval(scope);
        }
    }

    if (!Is<Number>(second)) {
        throw RuntimeError("Index expected as second argument in list_ref");
    }
    if (As<Number>(second)->GetValue() < 0) {
        throw RuntimeError("Out of range");
    }
    size_t ind = As<Number>(second)->GetValue();

    std::vector<std::shared_ptr<Object>> arr = GetArgs(first);
    //    while (first != nullptr) {
    //        arr.emplace_back(As<Cell>(first)->GetFirst());
    //        first = As<Cell>(first)->GetSecond();
    //    }
    if (ind >= arr.size()) {
        throw RuntimeError("Out of range");
    }
    return arr[ind];
}

std::shared_ptr<Object> OperationListTail::operator()(std::shared_ptr<Scope> scope,
                                                      std::shared_ptr<Object> args) {
    if (CountArgs(args) != 2) {
        throw RuntimeError("Two arguments expected in list_tail operation");
    }

    auto first = As<Cell>(args)->GetFirst();
    if (first != nullptr) {
        first = first->Eval(scope);
    }
    auto second = As<Cell>(args)->GetSecond();
    if (second != nullptr) {
        second = As<Cell>(second)->GetFirst();
        if (second != nullptr) {
            second = second->Eval(scope);
        }
    }

    if (!Is<Number>(second)) {
        throw RuntimeError("Index expected as second argument in list_tail");
    }
    if (As<Number>(second)->GetValue() < 0) {
        throw RuntimeError("Out of range");
    }
    size_t ind = As<Number>(second)->GetValue();

    std::vector<std::shared_ptr<Object>> arr = GetArgs(first);
    //    while (first != nullptr) {
    //        arr.emplace_back(As<Cell>(first)->GetFirst());
    //        first = As<Cell>(first)->GetSecond();
    //    }
    if (ind > arr.size()) {
        throw RuntimeError("Out of range");
    }
    if (ind == arr.size()) {
        return nullptr;
    }
    auto new_list = std::make_shared<Cell>(arr[ind], nullptr);
    auto cur = new_list;
    for (size_t i = ind + 1; i < arr.size(); ++i) {
        auto new_node = std::make_shared<Cell>(arr[i], nullptr);
        cur->GetSecond() = new_node;
        cur = new_node;
    }
    return new_list;
}

std::shared_ptr<Object> DefineCommand::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    auto args_v = GetArgs(args);

    if (args_v.size() < 2) {
        throw SyntaxError("At least two arguments expected in define");
    }

    auto first = As<Cell>(args)->GetFirst();
    if (!Is<Symbol>(first)) {
        //        throw SyntaxError("Symbol expected in define");
        auto signature_argv = GetArgs(As<Cell>(args)->GetFirst());
        auto body = GetArgs(As<Cell>(As<Cell>(args)->GetSecond()));
        scope->SetVariable(
            As<Symbol>(signature_argv[0])->GetName(),
            std::make_shared<LambdaCallback>(
                scope, std::vector(signature_argv.begin() + 1, signature_argv.end()), body));
        return nullptr;
    }

    if (args_v.size() != 2) {
        throw SyntaxError("Two arguments expected in define");
    }
    auto second = As<Cell>(args)->GetSecond();
    if (second != nullptr) {
        second = As<Cell>(second)->GetFirst();
        if (second != nullptr) {
            second = second->Eval(scope);
        }
    }
    scope->SetVariable(As<Symbol>(first)->GetName(), second);
    return nullptr;
}

std::shared_ptr<Object> SetCommand::operator()(std::shared_ptr<Scope> scope,
                                               std::shared_ptr<Object> args) {
    if (CountArgs(args) != 2) {
        throw SyntaxError("Two arguments expected in set");
    }

    auto first = As<Cell>(args)->GetFirst();
    if (!Is<Symbol>(first)) {
        throw SyntaxError("Symbol expected in set");
    }
    auto second = As<Cell>(args)->GetSecond();
    if (second != nullptr) {
        second = As<Cell>(second)->GetFirst();
        if (second != nullptr) {
            second = second->Eval(scope);
        }
    }
    //    auto var_scope = scope->FindScopeWithVariable(As<Symbol>(first)->GetName());
    //    As<Scope>(var_scope)->SetVariable(As<Symbol>(first)->GetName(), second);
    scope->FindVariable(As<Symbol>(first)->GetName()) = second;
    return nullptr;
}

std::shared_ptr<Object> IfConditional::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    size_t argc = CountArgs(args);
    if (argc < 2 || 3 < argc) {
        throw SyntaxError("Two or three arguments expected in if conditional");
    }

    auto ptr = As<Cell>(args)->GetFirst();
    auto argv = GetArgs(args);
    //    argv.emplace_back(As<Cell>(args)->GetFirst());
    //    ptr = As<Cell>(args)->GetSecond();
    //    while (ptr != nullptr) {
    //        argv.emplace_back(As<Cell>(ptr)->GetFirst());
    //        ptr = As<Cell>(ptr)->GetSecond();
    //    }
    assert(argv.size() == argc);

    auto res = argv[0]->Eval(scope);
    if (IsTrue(res)) {
        return argv[1]->Eval(scope);
    }
    if (argc == 3) {
        return argv[2]->Eval(scope);
    }
    return nullptr;
}

std::shared_ptr<Object> CreateLambda::operator()(std::shared_ptr<Scope> scope,
                                                 std::shared_ptr<Object> args) {
    if (CountArgs(args) < 2) {
        throw SyntaxError("At least two arguments expected in lambda creation");
    }

    if (!Is<Cell>(As<Cell>(args)->GetFirst()) && As<Cell>(args)->GetFirst() != nullptr) {
        throw SyntaxError("Arguments for lambda expected in lambda creation");
    }

    auto argv = GetArgs(As<Cell>(args)->GetFirst());
    for (auto arg : argv) {
        if (!Is<Symbol>(arg)) {
            throw SyntaxError("Only symbols in lambda signature");
        }
    }
    auto body = GetArgs(As<Cell>(args)->GetSecond());

    return std::make_shared<LambdaCallback>(scope, argv, body);
}

std::shared_ptr<Object> LambdaCallback::operator()(std::shared_ptr<Scope> scope,
                                                   std::shared_ptr<Object> args) {

    auto args_v = GetArgs(args);
    if (args_v.size() != argv_.size()) {
        throw RuntimeError("Not enough arguments");
    }
    auto lambda_scope = std::make_shared<Scope>(par_scope_);
    for (size_t i = 0; i < argv_.size(); ++i) {
        lambda_scope->SetVariable(As<Symbol>(argv_[i])->GetName(), args_v[i]->Eval(scope));
    }
    std::shared_ptr<Object> result = nullptr;
    for (size_t i = 0; i < body_.size(); ++i) {
        //        auto old_var = scope_->GetAllVars();
        result = body_[i]->Eval(lambda_scope);
        //        scope_->SetAllVars(old_var);
    }
    return result;
}
LambdaCallback::LambdaCallback(const std::shared_ptr<Scope>& scope,
                               const std::vector<std::shared_ptr<Object>>& argv,
                               const std::vector<std::shared_ptr<Object>>& body)
    : par_scope_(scope), argv_(argv), body_(body) {
}

std::shared_ptr<Object> SetCarCommand::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    if (CountArgs(args) != 2) {
        throw SyntaxError("Two arguments expected in set-car");
    }

    auto pair = As<Cell>(args)->GetFirst();
    if (pair != nullptr) {
        pair = pair->Eval(scope);
    }
    if (pair == nullptr) {
        throw RuntimeError("null result in set-car");
    }
    if (!Is<Cell>(pair)) {
        pair = pair->Eval(scope);
    }
    auto value = As<Cell>(As<Cell>(args)->GetSecond())->GetFirst();
    while (Is<Cell>(value)) {
        value = value->Eval(scope);
    }
    if (Is<Number>(value->Eval(scope))) {
        value = value->Eval(scope);
    }
    As<Cell>(pair)->GetFirst() = value;
    return pair;
}

std::shared_ptr<Object> SetCdrCommand::operator()(std::shared_ptr<Scope> scope,
                                                  std::shared_ptr<Object> args) {
    if (CountArgs(args) != 2) {
        throw SyntaxError("Two arguments expected in set-cdr");
    }

    auto pair = As<Cell>(args)->GetFirst();
    if (pair != nullptr) {
        pair = pair->Eval(scope);
    }
    if (pair == nullptr) {
        throw RuntimeError("null result in set-cdr");
    }
    if (!Is<Cell>(pair)) {
        pair = pair->Eval(scope);
    }
    auto value = As<Cell>(As<Cell>(args)->GetSecond())->GetFirst();
    while (Is<Cell>(value)) {
        value = value->Eval(scope);
    }
    if (Is<Number>(value->Eval(scope))) {
        value = value->Eval(scope);
    }

    As<Cell>(pair)->GetSecond() = value;
    return pair;
}
