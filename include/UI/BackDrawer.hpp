#pragma once
#include "widget.hpp"

namespace Squishy {
    struct BackDrawer {
        // Args
        squi::Widget::Args widget;
        squi::Children children{};
    
        class Impl : public squi::Widget {
            // Data
    
        public:
            Impl(const BackDrawer &args);

            void updateChildren() override;
        };
    
        operator squi::Child() const {
            return std::make_shared<Impl>(*this);
        }
    };
}