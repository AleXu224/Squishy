#include "editorItem.hpp"
#include "widgets/container.hpp"
#include "widgets/stack.hpp"
#include "widgets/text.hpp"

using namespace squi;

[[nodiscard]] squi::core::Child UI::EditorItem::build(const Element &) const {
	auto newWidget = widget;
	newWidget.height = newWidget.height.value_or(Size::Shrink);

	return Stack{
		.widget = newWidget,
		.children{
			Container{
				.widget{
					.width = Size::Shrink,
					.height = 32.f,
				},
				.child = Text{
					.widget{.alignment = Alignment::CenterLeft},
					.text = name,
				},
			},
			Container{
				.widget{
					.width = Size::Wrap,
					.height = Size::Wrap,
					.alignment = Alignment::CenterRight,
				},
				.child = child,
			},
		},
	};
}
