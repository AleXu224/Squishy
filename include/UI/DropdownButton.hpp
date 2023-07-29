#pragma once
#include "button.hpp"
#include "widget.hpp"

namespace Squishy {
    struct DropdownButton {
        // Args
        squi::Widget::Args widget{};
        squi::ButtonStyle style{};
    
        struct Storage {
            // Data
            squi::ButtonStyle style{};
            bool disabled = false;
        };
    
        operator squi::Child() const;
    };
}