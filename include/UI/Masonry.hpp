#pragma once
#include "widget.hpp"

namespace Squishy {
    struct Masonry {
        // Args
        squi::Widget::Args widget;
        float spacing = 0.0f;
        float minChildWidth = 100.0f;
        squi::Children children;
    
        class Impl : public squi::Widget {
            // Data
            float spacing;
            float minChildWidth;

            squi::vec2 layoutChildren(squi::vec2 maxSize, squi::vec2 minSize, ShouldShrink shouldShrink) final;
            void arrangeChildren(squi::vec2 &pos) final;

		public:
            Impl(const Masonry &args);
        };
    
        operator squi::Child() const {
            return {std::make_shared<Impl>(*this)};
        }
    };
}