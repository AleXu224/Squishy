#pragma once
#include "widget.hpp"
#include "statSheet.hpp"

namespace Squishy {
	struct StatsDisplay {
		// Args
		squi::Widget::Args widget;
		StatSheet &sheet;

		operator squi::Child() const;
	};

	struct CharacterCard {
        // Args
        StatSheet &sheet;
    
        struct Storage {
            // Data
			StatSheet &stats;
		};

		operator squi::Child() const;
    };
}