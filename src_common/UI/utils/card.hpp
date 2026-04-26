#pragma once

#include "color.hpp"
#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct Card : StatelessWidget {
		// Args
		Key key;
		Args widget;
		std::optional<squi::Color> borderColor{};
		squi::Child child{};

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI