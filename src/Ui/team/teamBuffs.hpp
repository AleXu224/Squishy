#pragma once

#include "widget.hpp"

#include "team/key.hpp"

namespace UI {
	struct TeamBuffs {
		// Args
		squi::Widget::Args widget{};
		Team::InstanceKey instanceKey;

		operator squi::Child() const;
	};
}// namespace UI