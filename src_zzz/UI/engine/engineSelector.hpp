#pragma once

#include "core/core.hpp"
#include "misc/specialty.hpp"
#include "observer.hpp"
#include "engine/key.hpp"


namespace UI {
	using namespace squi;
	struct EngineSelector : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		std::optional<Misc::Specialty> specialty{};
		std::function<void(Engine::DataKey)> onSelect{};

		struct State : WidgetState<EngineSelector> {
			VoidObservable closeEvent{};
			std::unordered_map<Misc::Specialty, bool> specialties{};

			Child build(const Element &element) override;
		};
	};
}// namespace UI