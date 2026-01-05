#pragma once

#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct EditorItem : StatelessWidget {
		// Args
		Key key;
		Args widget;
		std::string name;
		squi::Child child;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI