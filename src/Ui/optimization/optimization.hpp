#pragma once

#include "character/key.hpp"
#include "combo/source.hpp"
#include "enemy/key.hpp"
#include "team/key.hpp"
#include "widget.hpp"


namespace UI {
	struct Optimization {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;
		std::optional<Team::InstanceKey> teamKey{};
		Enemy::Key enemyKey{};

		struct Storage {
			// Data
			std::optional<Combo::Source::Types> nodeSource;
		};

		operator squi::Child() const;
	};
}// namespace UI