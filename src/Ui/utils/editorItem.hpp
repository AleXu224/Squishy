#pragma once

#include "widget.hpp"

namespace UI {
	struct EditorItem {
		// Args
		squi::Widget::Args widget{};
		std::string_view name;
		squi::Child child;

		operator squi::Child() const;
	};
}// namespace UI