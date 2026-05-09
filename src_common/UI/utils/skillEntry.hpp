#pragma once

#include "color.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct SkillEntry : StatelessWidget {
		// Args
		Key key;
		bool isTransparent;
		std::string name;
		std::optional<std::string> nameSuffix{};
		std::optional<float> value{};
		squi::Color color;
		squi::Color nameSuffixColor = Color::white;
		bool isPercentage = false;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI