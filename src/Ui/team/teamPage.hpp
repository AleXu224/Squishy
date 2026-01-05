#pragma once

#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct TeamPage : StatefulWidget {
		// Args
		Key key;
		Args widget{};

		struct State : WidgetState<TeamPage> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI