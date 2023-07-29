#pragma once
#include "vector"
#include "string"
#include <variant>

namespace Squishy {
    struct ValueList {
        size_t value;
        std::vector<float> values;
    };
    struct StringList {
        size_t value;
        std::vector<std::string> values;
    };

    using Conditional = std::variant<int, ValueList, StringList>;
}