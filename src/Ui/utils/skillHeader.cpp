#include "skillHeader.hpp"
#include "widgets/box.hpp"
#include "widgets/column.hpp"
#include "widgets/text.hpp"

using namespace squi;

[[nodiscard]] squi::core::Child UI::SkillHeader::build(const Element &) const {
	return Box{
		.widget{
			.height = Size::Shrink,
			.padding = Padding{16.f},
		},
		.color{1.f, 1.f, 1.f, 0.05f},
		.borderRadius{7.f, 7.f, 0.f, 0.f},
		.child = Column{
			.widget{
				.alignment = Alignment::CenterLeft,
			},
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
	};
}
