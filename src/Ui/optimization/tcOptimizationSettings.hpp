#pragma once

#include "widget.hpp"
#include "character/instance.hpp"

namespace UI {
	struct TCOptimizationSettings {
		// Args
		squi::Widget::Args widget{};
        Character::Instance &character;
        Formula::Context ctx;

		operator squi::Child() const;
	};
}// namespace UI