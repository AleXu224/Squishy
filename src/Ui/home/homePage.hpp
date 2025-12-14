#pragma once

#include "core/core.hpp"

namespace UI {
	struct HomePage : squi::StatelessWidget {
		// Args
		squi::Key key;

		squi::Child build(const squi::Element &) const;
	};
}// namespace UI