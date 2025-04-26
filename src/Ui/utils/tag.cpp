#include "tag.hpp"
#include "box.hpp"
#include "text.hpp"

using namespace squi;

UI::Tag::operator squi::Child() const {
	return Box{
		.widget{
			.width = Size::Shrink,
			.height = Size::Shrink,
			.padding = 4.f,
		},
		.color = Color::css(0x0, 0.2f),
		.borderColor = Color::css(0x0, 0.1f),
		.borderWidth = 1.f,
		.borderRadius = 4.f,
		.borderPosition = Box::BorderPosition::inset,
		.child = Text{
			.text = sourceStr,
			.fontSize = 12.f,
			.color = Color::css(0xffffff, 0.75f),
		},
	};
}
