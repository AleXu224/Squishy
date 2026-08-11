#pragma once

#include "core/core.hpp"

#include "formula/descriptor.hpp"

namespace UI {
	using namespace squi;
	struct DescriptorInspector : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Formula::Descriptor descriptor;

		struct State : WidgetState<DescriptorInspector> {
			Child buildFormula(const Formula::Descriptor &descriptor, bool prominent = false);
			Children buildItems(const Element &element, const Formula::Descriptor &descriptor);

			Child build(const Element &element) override;
		};
	};
}// namespace UI
