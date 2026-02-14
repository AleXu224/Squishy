#include "teamBuffs.hpp"
#include "Ui/elementToColor.hpp"
#include "Ui/utils/masonry.hpp"

#include "store.hpp"
#include "teamCharacterBuffsCard.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/themeOverride.hpp"

using namespace squi;

squi::core::Child UI::TeamBuffs::State::build(const Element &element) {
	auto &team = Store::teams.at(widget->instanceKey);

	Children teamCharacters{};

	auto theme = Theme::of(element);

	for (const auto &character: team.stats.characters) {
		if (!character) continue;

		auto newTheme = theme;
		newTheme.accent = Utils::elementToColor(character->state.stats.base.element);

		teamCharacters.emplace_back(ThemeOverride{
			.theme = newTheme,
			.child = UI::TeamCharacterBuffsCard{
				.team = team,
				.character = *character,
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
