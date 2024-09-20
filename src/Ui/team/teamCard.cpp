#include "teamCard.hpp"

#include "Ui/utils/card.hpp"
#include "Ui/utils/skillHeader.hpp"
#include "character/data.hpp"

#include "box.hpp"
#include "column.hpp"
#include "container.hpp"
#include "image.hpp"
#include "row.hpp"
#include "store.hpp"


using namespace squi;

struct TeamAvatar {
	// Args
	std::optional<std::reference_wrapper<Character::Instance>> character;

	operator squi::Child() const {

		return Box{
			.widget{
				.width = 64.f,
				.height = 64.f,
			},
			.color{1.f, 1.f, 1.f, 0.1f},
			.borderRadius{4.f},
			.child = character.has_value()
						 ? Image{
							   .fit = Image::Fit::contain,
							   .image = ImageProvider::fromFile(std::format("assets/Characters/{}/avatar.png", character->get().loadout.character.data.name))
						   }
						 : Child{},
		};
	}
};

struct TeamContents {
	// Args
	Team::InstanceKey teamKey;

	operator squi::Child() const {
		auto &team = ::Store::teams.at(teamKey);

		return Column{
			.children{
				UI::SkillHeader{
					.name = team.name,
				},
				Row{
					.widget{
						.height = Size::Shrink,
						.padding = 8.f,
					},
					.children{
						TeamAvatar{.character = team.stats.characters.at(0)},
						Container{.widget{.height = 0.f}},
						TeamAvatar{.character = team.stats.characters.at(1)},
						Container{.widget{.height = 0.f}},
						TeamAvatar{.character = team.stats.characters.at(2)},
						Container{.widget{.height = 0.f}},
						TeamAvatar{.character = team.stats.characters.at(3)},
					},
				},
			},
		};
	}
};

UI::TeamCard::operator squi::Child() const {
	return Card{
		.widget{
			.padding = Padding{1.f},
			.onInit = [teamKey = teamKey](Widget &w) {
				w.customState.add(::Store::teams.at(teamKey).updateEvent.observe([teamKey, &w]() {
					w.setChildren({TeamContents{.teamKey = teamKey}});
				}));
			},
		},
		.child = TeamContents{
			.teamKey = teamKey,
		},
	};
}
