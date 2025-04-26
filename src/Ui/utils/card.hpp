#pragma once

#include "color.hpp"
#include "widget.hpp"

namespace UI {
	struct Card {
		// Args
		squi::Widget::Args widget{};
		std::optional<squi::Color> borderColor{};
		squi::Child child{};

		operator squi::Child() const;
	};
}// namespace UI