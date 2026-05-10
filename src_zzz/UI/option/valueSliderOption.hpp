#pragma once

#include "core/core.hpp"

#include "agent/key.hpp"
#include "option/valueSlider.hpp"
#include "team/key.hpp"


namespace UI {
	using namespace squi;
	struct ValueSliderOption : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Option::ValueSlider &option;
		std::variant<Agent::InstanceKey, Team::InstanceKey> instanceKey{};
		std::function<void(float)> onChange;

		Formula::Context ctx;

		struct State : WidgetState<ValueSliderOption> {
			Children mods;

			void initState() override;

			void widgetUpdated() override;

			Child build(const Element &element) override;
		};
	};
}// namespace UI