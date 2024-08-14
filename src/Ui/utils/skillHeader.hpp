#pragma once

#include "widget.hpp"

namespace UI {
	struct SkillHeader {
		// Args
		squi::Widget::Args widget{};
		std::string_view name;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI