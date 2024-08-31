#include "skillEntry.hpp"
#include "align.hpp"
#include "box.hpp"
#include "stack.hpp"
#include "text.hpp"

using namespace squi;
UI::SkillEntry::operator squi::Child() const {
	return Box{
		.widget{
			.height = 36.f,
			.margin = Margin{0.f},
			.padding = Padding{12.f, 0.f},
		},
		.color = isTransparent ? Color{1.f, 1.f, 1.f, 0.0419f} : Color{0.f, 0.f, 0.f, 0.f},
		.borderRadius{4.f},
		.child{
			Stack{
				.children{
					Align{
						.xAlign = 0.f,
						.child = Text{
							.text = name,
							.color = color,
						},
					},
					Align{
						.xAlign = 1.f,
						.child = Text{
							.text = isPercentage ? std::format("{:.1f}%", value * 100.f) : std::format("{:.0f}", value),
						},
					},
				},
			},
		}
	};
}
