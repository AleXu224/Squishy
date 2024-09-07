#include "skillEntry.hpp"

#include "box.hpp"
#include "container.hpp"
#include "row.hpp"
#include "text.hpp"

using namespace squi;
UI::SkillEntry::operator squi::Child() const {
	return Box{
		.widget{
			.height = Size::Shrink,
			.sizeConstraints{
				.minHeight = 36.f,
			},
			.margin = Margin{0.f},
			.padding = Padding{12.f, 4.f},
		},
		.color = isTransparent ? Color{1.f, 1.f, 1.f, 0.0419f} : Color{0.f, 0.f, 0.f, 0.f},
		.borderRadius{4.f},
		.child{
			Row{
				.alignment = squi::Row::Alignment::center,
				.spacing = 4.f,
				.children{
					Container{
						.widget{
							.height = Size::Shrink,
						},
						.child = Text{
							.text = name,
							.lineWrap = true,
							.color = color,
						},
					},
					Text{
						.text = isPercentage ? std::format("{:.1f}%", value * 100.f) : std::format("{:.0f}", value),
					},
				},
			},
		}
	};
}
