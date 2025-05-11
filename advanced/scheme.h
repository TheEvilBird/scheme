#pragma once

// #define SCHEME_FUZZING_2_PRINT_REQUESTS

#include "tokenizer.h"
#include "parser.h"
#include "object.h"
#include "error.h"
#include <string>
#include <sstream>

class Interpreter {
public:
    Interpreter();

    std::string Run(const std::string& str);

private:
    std::shared_ptr<Scope> global_;
};
