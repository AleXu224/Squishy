#pragma once

#include "core/core.hpp"

#include "formula/descriptor.hpp"
#include "widgets/navigator.hpp"


namespace UI {
	using namespace squi;
	struct DescriptorDisplay : StatefulWidget {
		// Args
		Key key;
		std::function<Formula::Descriptor()> descriptorProvider;
		Child child;

		struct State : WidgetState<DescriptorDisplay> {
			Key descriptorDisplayKey = GlobalKey{};

			void dispose() override {
				Navigator::of(this).popOverlay(descriptorDisplayKey);
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI