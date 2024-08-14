#pragma once

#include "color.hpp"
#include "widget.hpp"


namespace UI {
	struct SkillEntry {
		// Args
		bool isTransparent;
		std::string_view name;
		float value;
		squi::Color color;
		bool isPercentage = false;

		operator squi::Child() const;
	};
}// namespace UI