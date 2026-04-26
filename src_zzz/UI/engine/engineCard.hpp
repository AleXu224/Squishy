#pragma once

#include "../../engine/instance.hpp"
#include "core/core.hpp"
#include "cstdint"


namespace UI {
	using namespace squi;
	struct EngineCard : StatefulWidget {
		enum class Actions : uint8_t {
			list,
			agent,
			showcase,
		};

		// Args
		Key key;
		Args widget{};
		Engine::Instance *engine;
		Agent::InstanceKey agentKey{};
		Actions actions = EngineCard::Actions::list;

		struct State : WidgetState<EngineCard> {
			VoidObserver engineUpdateObserver{};

			void observeEngine() {
				if (!widget->engine) {
					engineUpdateObserver = VoidObserver{};
					return;
				}
				engineUpdateObserver = widget->engine->updateEvent.observe([this]() {
					setState([]() {});
				});
			}

			void initState() override {
				observeEngine();
			}

			void widgetUpdated() override {
				observeEngine();
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI