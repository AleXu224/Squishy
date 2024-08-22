#include "teamCard.hpp"

#include "Ui/utils/card.hpp"
#include "store.hpp"

#include "column.hpp"
#include "row.hpp"
#include "text.hpp"


using namespace squi;

struct TeamCardBanner {
	// Args
	Stats::Team &team;

	operator squi::Child() const {
		return Row{
			.widget{
				.height = 64.f,
			},
			.children{
				Column{
					.widget{
						.padding = Padding{4.f},
					},
					.spacing = 4.f,
					.children{
						Text{
							.text = std::format("Team #"),
							.fontSize = 24.f,
						},
					},
				},
			},
		};
	}
};

struct TeamContents {
	// Args
	Stats::Team &team;

	operator squi::Child() const {
		return Column{
			.children{
				TeamCardBanner{
					.team = team,
				},
			},
		};
	}
};

UI::TeamCard::operator squi::Child() const {
	return Card{
		.widget{
			.padding = Padding{1.f},
			// .onInit = [teamKey = teamKey](Widget &w) {
			// 	w.customState.add(Store::characters.at(characterKey).updateEvent.observe([characterKey, &w]() {
			// 		w.setChildren({Contents{.character = Store::characters.at(characterKey)}});
			// 	}));
			// },
		},
		.child = TeamContents{
			.team = Store::teams.at(teamKey),
		},
	};
}
