#pragma once

#include "widget.hpp"

namespace UI {
	struct Card {
		// Args
		squi::Widget::Args widget{};
        squi::Child child{};

		operator squi::Child() const;
	};
}// namespace UI