#pragma once
#include "widget.hpp"

namespace Squishy {
    struct Grid {
        // Args
        squi::Widget::Args widget;
        float spacing = 0.f;
        float minChildWidth = 100.f;
        squi::Children children{};
    
        class Impl : public squi::Widget {
            // Data
            float spacing;
            float minChildWidth;
    
        public:
            Impl(const Grid &args);

            squi::vec2 layoutChildren(squi::vec2 maxSize, squi::vec2 minSize, ShouldShrink shouldShrink) final;
            void arrangeChildren(squi::vec2 &pos) final;
		};
    
        operator squi::Child() const {
            return std::make_shared<Impl>(*this);
        }
    };
}