#include "teamBuffs.hpp"
#include "Ui/utils/masonry.hpp"

#include "store.hpp"
#include "teamCharacterBuffsCard.hpp"
#include "widgets/scrollview.hpp"

using namespace squi;

[[nodiscard]] squi::core::Child UI::TeamBuffs::build(const Element &) const {
	auto &team = Store::teams.at(instanceKey);

	Children teamCharacters{};

	for (const auto &character: team.stats.characters) {
		if (!character) continue;

		teamCharacters.emplace_back(UI::TeamCharacterBuffsCard{
			.team = team,
			.character = *character,
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
