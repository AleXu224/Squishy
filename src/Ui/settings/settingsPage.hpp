#pragma once

#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct SettingsPage : StatefulWidget {
		// Args
		Key key;

		struct State : WidgetState<SettingsPage> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI