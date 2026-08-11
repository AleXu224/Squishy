#include "descriptorDisplay.hpp"

#include "widgets/gestureDetector.hpp"
#include "widgets/navigator.hpp"
#include "widgets/richText.hpp"
#include "widgets/tooltip.hpp"

#include "UI/formula/descriptorInspector.hpp"

namespace UI {
	squi::core::Child DescriptorDisplay::State::build(const Element &element) {
		return Gesture{
			.onClick = [this](const Gesture::State &) {
				Navigator::of(this).pushOverlay(DescriptorInspector{
					.key = descriptorDisplayKey,
					.descriptor = widget->descriptorProvider(),
				});
			},
			.requirements = InputLevel::click,
			.child = UI::Tooltip{
				.text = [this]() {
					return RichText{
						.text = widget->descriptorProvider().spans | std::ranges::to<std::vector>(),
						.lineWrap = true,
					};
				},
				.child = widget->child,
			},
		};
	}
}// namespace UI