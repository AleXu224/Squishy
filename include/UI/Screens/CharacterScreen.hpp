#pragma once
#include "character.hpp"
#include "child.hpp"
#include "widget.hpp"
#include <optional>

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