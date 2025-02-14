#pragma once

#include "widget.hpp"

struct CustomTooltip {
	// Args
	squi::Widget::Args widget{};
	squi::Child tooltip;
	squi::Child child{};

	struct Storage {
		// Data
	};

	operator squi::Child() const;
};