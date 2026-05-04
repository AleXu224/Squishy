#include "teamBuffs.hpp"
#include "UI/attributeToColor.hpp"
#include "UI/utils/masonry.hpp"

#include "store.hpp"
#include "teamAgentBuffsCard.hpp"
#include "widgets/builder.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/themeOverride.hpp"

using namespace squi;

squi::core::Child UI::TeamBuffs::State::build(const Element &element) {
	auto &team = Store::teams.at(widget->instanceKey);

	Children teamAgents{};

	auto theme = Theme::of(element);

	for (const auto &[index, agent]: team.stats.agents | std::views::enumerate) {
		if (!agent) continue;

		auto newTheme = theme;
		newTheme.accent = Utils::attributeToColor(agent->state.stats.base.attribute);

		teamAgents.emplace_back(ThemeOverride{
			.theme = newTheme,
			.child = Column{
				.widget{
					.width = Size::Wrap,
					.height = Size::Wrap,
				},
				.spacing = 4.f,
				.children{
					Builder{
						.builder = [this, index, &team](const Element &element) {
							return Button{
								.widget{
									.width = Size::Expand,
								},
								.theme = Button::Theme::Accent(element),
								.disabled = index == team.stats.activeAgentIndex,
								.onClick = [this, index]() {
									auto &team = Store::teams.at(widget->instanceKey);
									team.stats.activeAgentIndex = index;
									team.updateEvent.notify();
								},
								.child = "Make Active",
							};
						},
					},
					UI::TeamAgentBuffsCard{
						.team = team,
						.agent = *agent,
					},
				},
			},
		});
	}
	return ScrollView{
		.alignment = Flex::Alignment::center,
		.children{
			UI::Masonry{
				.widget{
					.width = Size::Wrap,
					.padding = 8.f,
				},
				.columnCount = UI::Masonry::MinSize{250.f},
				.spacing = 4.f,
				.children = teamAgents,
			}
		},
	};
}
