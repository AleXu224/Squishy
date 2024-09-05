#pragma once

#include "character/key.hpp"
#include "enemy/key.hpp"
#include "team/key.hpp"


#include "widget.hpp"

namespace UI {
	struct CharacterDetails {
		// Args
		squi::Widget::Args widget{};
		Character::Key characterKey{};
		Team::Key teamKey{};
		Enemy::Key enemyKey{};

		operator squi::Child() const;
	};
}// namespace UI