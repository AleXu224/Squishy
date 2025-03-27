#pragma once

#include "color.hpp"
#include "widgetArgs.hpp"


namespace UI {
	struct SkillEntry {
		// Args
		bool isTransparent;
		std::string_view name;
		std::optional<float> value{};
		squi::Color color;
		bool isPercentage = false;

		operator squi::Child() const;
	};
}// namespace UI