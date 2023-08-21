#pragma once
#include "widget.hpp"
#include <vector>

namespace Squishy {
    struct TopNav {
        struct Tab {
            std::string_view name;
            squi::Child child;
        };
        
        // Args
        squi::Widget::Args widget;
        const std::vector<Tab> tabs{};
    
        struct State {
            // Data
            squi::ChildRef selectedTab{};
        };
    
        operator squi::Child() const;
    };
}