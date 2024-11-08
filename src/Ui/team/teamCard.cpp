#include "teamCard.hpp"

#include "Ui/utils/card.hpp"
#include "Ui/utils/skillHeader.hpp"
#include "button.hpp"
#include "character/data.hpp"
#include "teamDetails.hpp"
#include "teamEditor.hpp"

#include "box.hpp"
#include "column.hpp"
#include "container.hpp"
#include "image.hpp"
#include "row.hpp"
#include "store.hpp"


using namespace squi;

struct TeamAvatar {
	// Args
	Character::Instance *character;

	operator squi::Child() const {

		return Box{
			.widget{
				.width = 64.f,
				.height = 64.f,
			},
			.color{1.f, 1.f, 1.f, 0.1f},
			.borderRadius{4.f},
			.child = character
					   ? Image{
							 .fit = Image::Fit::contain,
							 .image = ImageProvider::fromFile(std::format("assets/Characters/{}/avatar.png", character->loadout.character.data.name))
						 }
					   : Child{},
		};
	}
};

struct TeamContents {
	// Args
	Team::InstanceKey teamKey{};
	squi::Navigator::Controller controller;

	operator squi::Child() const {
		auto &team = ::Store::teams.at(teamKey);

		return Column{
			.children{
				GestureDetector{
					.onClick = [controller = controller, teamKey = teamKey](GestureDetector::Event) {
						controller.push(UI::TeamDetails{
							.teamKey = teamKey,
							.controller = controller,
						});
					},
					.child = UI::SkillHeader{
						.name = team.name,
					},
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
				Row{
					.widget{
						.height = Size::Shrink,
						.padding = 4.f,
					},
					.spacing = 4.f,
					.children{
						Button{
							.text = "Edit",
							.style = ButtonStyle::Standard(),
							.onClick = [teamKey = teamKey](GestureDetector::Event event) {
								event.widget.addOverlay(UI::TeamEditor{
									.instance = Store::teams.at(teamKey),
									.onSubmit = [](const Team::Instance &team) {
										Store::teams.at(team.instanceKey) = team;
										Store::teamListUpdateEvent.notify();
									},
								});
							},
						},
						Button{
							.text = "Delete",
							.style = ButtonStyle::Standard(),
							.onClick = [teamKey = teamKey](GestureDetector::Event) {
								Store::teams.erase(teamKey);
								Store::teamListUpdateEvent.notify();
							},
						},
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
			.onInit = [controller = controller, teamKey = teamKey](Widget &w) {
				w.customState.add(::Store::teams.at(teamKey).updateEvent.observe([controller, teamKey, &w]() {
					w.setChildren({TeamContents{.teamKey = teamKey, .controller = controller}});
				}));
			},
		},
		.child = TeamContents{
			.teamKey = teamKey,
			.controller = controller,
		},
	};
}
