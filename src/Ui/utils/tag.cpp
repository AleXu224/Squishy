#include "tag.hpp"

#include "widgets/box.hpp"
#include "widgets/text.hpp"

using namespace squi;

[[nodiscard]] squi::core::Child UI::Tag::build(const Element &) const {
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
