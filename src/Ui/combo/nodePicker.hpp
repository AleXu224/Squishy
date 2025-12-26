#pragma once

#include "core/core.hpp"

#include "combo/source.hpp"
#include "functional"
#include "observer.hpp"

namespace UI {
	using namespace squi;
	struct NodePicker : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;
		bool enableCombos = false;
		const Formula::Context &ctx;
		std::function<void(Combo::Source::Types)> onSelect;

		struct State : WidgetState<NodePicker> {
			VoidObservable closeEvent;

			Child build(const Element &element) override;
		};
	};
}// namespace UI