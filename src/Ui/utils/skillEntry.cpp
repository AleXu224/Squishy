#include "skillEntry.hpp"
#include "widgets/box.hpp"
#include "widgets/container.hpp"
#include "widgets/row.hpp"
#include "widgets/text.hpp"


using namespace squi;
[[nodiscard]] squi::core::Child UI::SkillEntry::build(const Element &) const {
	return Box{
		.widget{
			.height = Size::Shrink,
			.sizeConstraints = BoxConstraints{
				.minHeight = 36.f,
			},
			.margin = Margin{0.f},
			.padding = Padding{12.f, 4.f},
		},
		.color = isTransparent ? Color{1.f, 1.f, 1.f, 0.0419f} : Color{0.f, 0.f, 0.f, 0.f},
		.borderRadius{4.f},
		.child{
			Row{
				.crossAxisAlignment = Flex::Alignment::center,
				.spacing = 4.f,
				.children{
					Container{
						.widget{
							.height = Size::Shrink,
						},
						.child = Text{
							.text = std::string(name),
							.lineWrap = true,
							.color = color,
						},
					},
					value.has_value() ? Text{
											.text = isPercentage ? std::format("{:.1f}%", value.value() * 100.f) : std::format("{:.0f}", value.value()),
										}
									  : Child{},
				},
			},
		}
	};
}
