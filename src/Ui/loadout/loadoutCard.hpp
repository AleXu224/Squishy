#pragma once

#include "character/key.hpp"
#include "widget.hpp"


namespace UI {
	struct LoadoutCard {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;
		std::optional<uint32_t> loadoutIndex = std::nullopt;

		operator squi::Child() const;
	};
}// namespace UI