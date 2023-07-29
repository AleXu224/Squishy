#pragma once
#include "child.hpp"
#include "widget.hpp"

namespace Squishy {
    struct Card {
        // Args
        squi::Widget::Args widget;
        squi::Child child;

        operator squi::Child() const;
    };
}