#pragma once

#include "string_view"

namespace Conditional {
    struct Boolean {
        std::string_view key;
        std::string_view name;
        bool active = false;

        void toggle() {
            active = !active;
        }
    };
}