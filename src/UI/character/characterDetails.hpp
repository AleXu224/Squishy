#pragma once

#include "character/key.hpp"
#include "enemy/key.hpp"
#include "observer.hpp"
#include "store.hpp"
#include "team/key.hpp"


#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct CharacterDetails : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey{};
		std::optional<Team::InstanceKey> teamKey{};
		Enemy::Key enemyKey{};

		struct State : WidgetState<CharacterDetails> {
			VoidObserver characterUpdateEvent;
			VoidObserver loadoutUpdateEvent;

			void initState() override {
				characterUpdateEvent = ::Store::characters.at(widget->characterKey).updateEvent.observe([this]() {
					this->setState();
				});
				loadoutUpdateEvent = ::Store::characters.at(widget->characterKey).loadoutChangedEvent.observe([this]() {
					this->setState();
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI