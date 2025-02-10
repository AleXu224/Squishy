#include "skillHeader.hpp"
#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "text.hpp"

using namespace squi;

UI::SkillHeader::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Box{
		.widget{
			.height = Size::Shrink,
			.padding = Padding{16.f},
		},
		.color{1.f, 1.f, 1.f, 0.05f},
		.borderRadius{7.f, 7.f, 0.f, 0.f},
		.child = Align{
			.xAlign = 0.f,
			.child = Column{
				.spacing = 8.f,
				.children{
					Text{
						.text = name,
						.fontSize = 16.f,
						.lineWrap = true,
						.font = FontStore::defaultFontBold,
					},
					subtitle.empty()
						? Child{}
						: Text{
							  .text = subtitle,
							  .fontSize = 14.f,
							  .lineWrap = true,
							  .font = FontStore::defaultFont,
							  .color = Color::css(255, 255, 255, 0.8f),
						  },
				},
			},
		},
	};
}
