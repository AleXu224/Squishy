#pragma once

#include "observer.hpp"
#include "widget.hpp"

namespace UI {
	struct Tooltip {
		// Args
		float maxWidth = 600.f;
		std::string message{};
		squi::Child child{};

		struct State {
			squi::VoidObservable destroyEvent{};
		};

		operator squi::Child() const;
	};
}// namespace UI