#pragma once

#include "character/key.hpp"
#include "core/core.hpp"
#include "enemy/key.hpp"
#include "observer.hpp"
#include "store.hpp"
#include "team/key.hpp"


namespace UI {
	using namespace squi;
	struct TCOptimization : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;
		std::optional<Team::InstanceKey> teamKey{};
		Enemy::Key enemyKey{};

		struct State : WidgetState<TCOptimization> {
			VoidObserver characterUpdateEvent;

			void initState() override {
				auto &character = ::Store::characters.at(widget->characterKey);
				characterUpdateEvent = character.updateEvent.observe([this]() {
					setState([]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI