#pragma once

#include "core/core.hpp"

#include "widgets/richText.hpp"
#include "widgets/tooltip.hpp"

#include "formula/descriptor.hpp"

namespace UI {
	using namespace squi;
	struct DescriptorDisplay : StatefulWidget {
		// Args
		Key key;
		std::function<Formula::Descriptor()> descriptorProvider;
		Child child;

		struct State : WidgetState<DescriptorDisplay> {
			Child build(const Element &element) override {
				return UI::Tooltip{
					.text = [this]() {
						return RichText{
							.text = widget->descriptorProvider().spans | std::ranges::to<std::vector>(),
							.lineWrap = true,
						};
					},
					.child = widget->child,
				};
			}
		};
	};
}// namespace UI