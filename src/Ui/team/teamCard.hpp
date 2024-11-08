#pragma once

#include "team/key.hpp"
#include "widget.hpp"
#include "navigator.hpp"

namespace UI {
	struct TeamCard {
		// Args
		squi::Widget::Args widget{};
		Team::InstanceKey teamKey{};
		squi::Navigator::Controller controller;

		operator squi::Child() const;
	};
}// namespace UI