#include "card.hpp"
#include "widgets/box.hpp"

using namespace squi;

squi::core::Child UI::Card::build(const Element &) const {
	auto newWidget = widget;
	newWidget.padding = newWidget.padding.value_or(1.f);

	return Box{
		.widget = newWidget,
		.color{1.f, 1.f, 1.f, 0.0512f},
		.borderColor = borderColor.value_or(Color(0.f, 0.f, 0.f, 0.1f)),
		.borderWidth = borderColor.has_value() ? 2.f : 1.f,
		.borderRadius{8.f},
		.borderPosition = squi::Box::BorderPosition::outset,
		.child = child,
	};
}
