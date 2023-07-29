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

            void layoutChildren(squi::vec2 &maxSize, squi::vec2 &minSize) final;
            void arrangeChildren(squi::vec2 &pos) final;

			float getMinHeight(const squi::vec2 &maxSize) final;

		public:
            Impl(const Masonry &args);
        };
    
        operator squi::Child() const {
            return {std::make_shared<Impl>(*this)};
        }
    };
}