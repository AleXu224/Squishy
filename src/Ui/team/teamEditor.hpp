#pragma once

#include "widget.hpp"

#include "team/instance.hpp"

namespace UI {
    struct TeamEditor {
        // Args
        squi::Widget::Args widget{};
        Team::Instance instance;
        std::function<void(const Team::Instance &)> onSubmit{};
    
        struct Storage {
            // Data
            Team::Instance team;
        };
    
        operator squi::Child() const;
    };
}