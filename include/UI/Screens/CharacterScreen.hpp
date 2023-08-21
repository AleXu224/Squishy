#pragma once
#include "widget.hpp"

namespace Squishy {
    struct CharacterScreen {
        // Args
        squi::Widget::Args widget;
    
        struct State {
            // Data
            squi::Child screenToOpen{};
        };
    
        operator squi::Child() const;
    };
}