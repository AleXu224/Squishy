#pragma once

#include "core/core.hpp"

#include "artifact/key.hpp"
#include "character/instance.hpp"

namespace UI {
	using namespace squi;
	struct OptimizationSetChooser : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::Instance &character;
		Formula::Context ctx;
		std::unordered_map<Artifact::SetKey, bool> &twoPcSets;
		std::unordered_map<Artifact::SetKey, bool> &fourPcSets;

		struct State : WidgetState<OptimizationSetChooser> {
			VoidObservable closeEvent;
			VoidObserver characterUpdateEvent;

			void initState() override {
				characterUpdateEvent = widget->character.updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI