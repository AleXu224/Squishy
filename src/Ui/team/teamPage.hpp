#pragma once

#include "navigator.hpp"
#include "team/key.hpp"
#include "widget.hpp"


namespace UI {
	struct TeamPage {
		// Args
		squi::Widget::Args widget{};
		Team::Key teamKey{};
		squi::Navigator::Controller controller;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI