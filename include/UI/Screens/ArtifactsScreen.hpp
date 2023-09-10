#pragma once

#include "widget.hpp"

namespace Squishy {
    struct ArtifactsScreen {
        // Args
        squi::Widget::Args widget;
    
        struct Storage {
            // Data
        };

		operator squi::Child() const;
	};
}