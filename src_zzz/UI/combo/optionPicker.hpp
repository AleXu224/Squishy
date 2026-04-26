#pragma once

#include "core/core.hpp"

#include "agent/key.hpp"
#include "combo/option.hpp"
#include "formula/base.hpp"
#include "observer.hpp"

namespace UI {
	using namespace squi;
	struct OptionPicker : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;
		Formula::Context ctx;
		const std::vector<::Combo::Option> &options;
		std::function<void(::Combo::Option)> onSelect;

		struct State : WidgetState<OptionPicker> {
			VoidObservable closeEvent;

			Child build(const Element &element) override;
		};
	};
}// namespace UI