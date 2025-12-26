#pragma once

#include "character/key.hpp"
#include "core/core.hpp"
#include "formula/formulaContext.hpp"
#include "observer.hpp"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct ComboList : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;
		Formula::Context ctx;

		struct State : WidgetState<ComboList> {
			VoidObservable closeEvent;
			VoidObserver characterUpdatedEvent;

			void initState() override {
				characterUpdatedEvent = ::Store::characters.at(widget->characterKey).updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI