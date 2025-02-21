#include "teamBuffs.hpp"
#include "Ui/utils/grid.hpp"
#include "rebuilder.hpp"
#include "scrollableFrame.hpp"

#include "store.hpp"
#include "teamCharacterBuffsCard.hpp"

using namespace squi;

namespace {
	Child makeContent(Team::InstanceKey instanceKey) {
		auto &team = Store::teams.at(instanceKey);

		Children teamCharacters{};

		for (const auto &character: team.stats.characters) {
			if (!character) continue;

			teamCharacters.emplace_back(UI::TeamCharacterBuffsCard{
				.team = team,
				.character = *character,
			});
		}

		return UI::Grid{
			.widget{
				.width = Size::Wrap,
				.padding = 8.f,
			},
			.spacing = 4.f,
			.columnCount = UI::Grid::MinSize{250.f},
			.children = teamCharacters,
		};
	}
}// namespace

UI::TeamBuffs::operator squi::Child() const {
	return ScrollableFrame{
		.alignment = squi::Column::Alignment::center,
		.children{
			Rebuilder{
				.widget{.width = Size::Wrap},
				.rebuildEvent = Store::teams.at(instanceKey).updateEvent,
				.buildFunc = std::bind(makeContent, instanceKey),
			},
		},
	};
}
