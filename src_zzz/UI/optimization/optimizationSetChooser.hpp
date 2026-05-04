#pragma once

#include "core/core.hpp"

#include "disc/key.hpp"
#include "agent/instance.hpp"

namespace UI {
	using namespace squi;
	struct OptimizationSetChooser : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::Instance &agent;
		Formula::Context ctx;
		std::unordered_map<Disc::SetKey, bool> &twoPcSets;
		std::unordered_map<Disc::SetKey, bool> &fourPcSets;

		struct State : WidgetState<OptimizationSetChooser> {
			VoidObservable closeEvent;
			VoidObserver agentUpdateEvent;

			void initState() override {
				agentUpdateEvent = widget->agent.updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			void setSet(std::unordered_map<Disc::SetKey, bool> &setMap, bool value);

			Child build(const Element &element) override;
		};
	};
}// namespace UI