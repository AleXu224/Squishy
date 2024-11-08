#pragma once

#include "widget.hpp"

namespace UI {
	struct TeamPage {
		// Args
		squi::Widget::Args widget{};

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI