#pragma once

#include "widget.hpp"

namespace UI {
	struct SkillHeader {
		// Args
		squi::Widget::Args widget{};
		std::string_view name;
		std::string_view subtitle{};

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI