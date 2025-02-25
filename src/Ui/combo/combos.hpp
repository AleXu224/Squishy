#pragma once

#include "widget.hpp"

namespace UI {
    struct Combos {
        // Args
        squi::Widget::Args widget{};
    
        struct Storage {
            // Data
        };
    
        operator squi::Child() const;
    };
}