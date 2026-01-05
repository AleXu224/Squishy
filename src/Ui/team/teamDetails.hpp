#pragma once

#include "core/core.hpp"
#include "observer.hpp"
#include "store.hpp"
#include "team/key.hpp"


namespace UI {
	using namespace squi;
	struct TeamDetails : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		bool enableBackButton = true;
		Team::InstanceKey teamKey{};

		struct State : WidgetState<TeamDetails> {
			VoidObserver teamUpdateEvent;

			void initState() override {
				auto &team = ::Store::teams.at(widget->teamKey);

				teamUpdateEvent = team.updateEvent.observe([this]() {
					setState([]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI