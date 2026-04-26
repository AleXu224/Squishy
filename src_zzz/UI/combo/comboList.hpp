#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"
#include "formula/base.hpp"
#include "observer.hpp"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct ComboList : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;
		Formula::Context ctx;

		struct State : WidgetState<ComboList> {
			VoidObservable closeEvent;
			VoidObserver agentUpdatedEvent;

			void initState() override {
				agentUpdatedEvent = ::Store::agents.at(widget->agentKey).updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI