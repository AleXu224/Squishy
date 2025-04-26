#include "card.hpp"
#include "box.hpp"

using namespace squi;

UI::Card::operator squi::Child() const {
	return Box{
		.widget = widget,
		.color{1.f, 1.f, 1.f, 0.0512f},
		.borderColor = borderColor.value_or(Color(0.f, 0.f, 0.f, 0.1f)),
		.borderWidth{1.f},
		.borderRadius{8.f},
		.borderPosition = squi::Box::BorderPosition::outset,
		.child = child,
	};
}
