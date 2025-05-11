#pragma once

#include "character/key.hpp"
#include "enemy/key.hpp"
#include "team/key.hpp"
#include "widget.hpp"


namespace UI {
	struct TCOptimization {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;
		std::optional<Team::InstanceKey> teamKey{};
		Enemy::Key enemyKey{};

		operator squi::Child() const;
	};
}// namespace UI