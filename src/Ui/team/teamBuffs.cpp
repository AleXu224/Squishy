#include "teamBuffs.hpp"
#include "Ui/elementToColor.hpp"
#include "Ui/utils/masonry.hpp"

#include "store.hpp"
#include "teamCharacterBuffsCard.hpp"
#include "widgets/builder.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/themeOverride.hpp"

using namespace squi;

squi::core::Child UI::TeamBuffs::State::build(const Element &element) {
	auto &team = Store::teams.at(widget->instanceKey);

	Children teamCharacters{};

	auto theme = Theme::of(element);

	for (const auto &[index, character]: team.stats.characters | std::views::enumerate) {
		if (!character) continue;

		auto newTheme = theme;
		newTheme.accent = Utils::elementToColor(character->state.stats.base.element);

		teamCharacters.emplace_back(ThemeOverride{
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
								.disabled = index == team.stats.activeCharacterIndex,
								.onClick = [this, index]() {
									auto &team = Store::teams.at(widget->instanceKey);
									team.stats.activeCharacterIndex = index;
									team.updateEvent.notify();
								},
								.child = "Make Active",
							};
						},
					},
					UI::TeamCharacterBuffsCard{
						.team = team,
						.character = *character,
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
				.children = teamCharacters,
			}
		},
	};
}
