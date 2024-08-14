#include "skillHeader.hpp"
#include "align.hpp"
#include "box.hpp"
#include "text.hpp"

using namespace squi;

UI::SkillHeader::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Box{
		.widget{
			.height = Size::Shrink,
			.padding = Padding{16.f},
		},
		.color{1.f, 1.f, 1.f, 0.1f},
		.borderRadius{7.f, 7.f, 0.f, 0.f},
		.child = Align{
			.xAlign = 0.f,
			.child = Text{
				.text = name,
				.fontSize = 20.f,
				.lineWrap = true,
				.font = FontStore::defaultFontBold,
			},
		},
	};
}
