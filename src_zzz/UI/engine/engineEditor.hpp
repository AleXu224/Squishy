#pragma once

#include "../../engine/engines.hpp"
#include "../../engine/instance.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct EngineEditor : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Engine::Instance engine;
		std::function<void(Engine::Instance)> onSubmit{};

		struct State : WidgetState<EngineEditor> {
			// Engine instance has no default constructor
			Engine::Instance engine = Engine::Instance{
				Engine::list.begin()->second.key,
				{0},
			};
			VoidObservable closeEvent{};

			void initState() override {
				engine = widget->engine;
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI