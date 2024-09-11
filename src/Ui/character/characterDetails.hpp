#pragma once

#include "character/key.hpp"
#include "enemy/key.hpp"
#include "team/key.hpp"


#include "widget.hpp"

namespace UI {
	struct CharacterDetails {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey{};
		Team::InstanceKey teamKey{};
		Enemy::Key enemyKey{};

		operator squi::Child() const;
	};
}// namespace UI