#pragma once

#include "widget.hpp"

namespace UI {
	struct CharacterPage {
		// Args
		squi::Widget::Args widget{};

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI