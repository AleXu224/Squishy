#pragma once

#include "widget.hpp"


namespace UI {
	struct TeamCard {
		// Args
		squi::Widget::Args widget{};
		uint32_t teamKey{};

		operator squi::Child() const;
	};
}// namespace UI