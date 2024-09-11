#pragma once

#include "team/key.hpp"
#include "widget.hpp"


namespace UI {
	struct TeamCard {
		// Args
		squi::Widget::Args widget{};
		Team::InstanceKey teamKey{};

		operator squi::Child() const;
	};
}// namespace UI