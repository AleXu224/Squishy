#pragma once

#include "core/core.hpp"

#include "observer.hpp"
#include "store.hpp"
#include "team/key.hpp"


namespace UI {
	using namespace squi;
	struct TeamBuffs : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Team::InstanceKey instanceKey;

		struct State : WidgetState<TeamBuffs> {
			VoidObserver teamUpdateEvent;

			void initState() override {
				auto &team = Store::teams.at(widget->instanceKey);

				teamUpdateEvent = team.updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI