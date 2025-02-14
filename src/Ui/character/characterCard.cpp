#include "characterCard.hpp"

#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "button.hpp"
#include "character/characters.hpp"
#include "characterEditor.hpp"
#include "characterPage.hpp"
#include "formula/stat.hpp"
#include "rebuilder.hpp"
#include "store.hpp"

#include "column.hpp"
#include "container.hpp"
#include "image.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "text.hpp"


using namespace squi;

struct CharacterCardBanner {
	// Args
	Character::InstanceKey characterKey;

	operator squi::Child() const {
		auto &character = ::Store::characters.at(characterKey);
		return Stack{
			.widget{
				.height = 64.f,
			},
			.children{
				Container{
					.child = Image{
						.fit = squi::Image::Fit::cover,
						.image = ImageProvider::fromFile(std::format("assets/Characters/{}/banner.png", Character::list.at(character.dataKey).name)),
					},
				},
				Row{
					.children{
						Image{
							.fit = squi::Image::Fit::contain,
							.image = ImageProvider::fromFile(std::format("assets/Characters/{}/avatar.png", Character::list.at(character.dataKey).name)),
						},
						Column{
							.widget{
								.padding = Padding{4.f},
							},
							.spacing = 4.f,
							.children{
								Text{
									.text = Character::list.at(character.dataKey).name,
									.fontSize = 24.f,
								},
								Text{
									.text = std::format("Lvl {} C{}", character.loadout.character.sheet.level, character.loadout.character.sheet.constellation),
								},
							},
						},
					},
				},
			},
		};
	}
};

struct Contents {
	// Args
	Character::InstanceKey characterKey;
	squi::Navigator::Controller controller;

	operator squi::Child() const {
		return Column{
			.children{
				GestureDetector{
					.onClick = [controller = controller, characterKey = characterKey](GestureDetector::Event) {
						controller.push(UI::CharacterPage{
							.characterKey = characterKey,
							.controller = controller,
						});
					},
					.child = CharacterCardBanner{
						.characterKey = characterKey,
					},
				},
				Column{
					.widget{
						.padding = Padding{4.f},
						.onInit = [characterKey = characterKey](Widget &w) {
							auto &character = ::Store::characters.at(characterKey);
							auto statsToDisplay = std::vector{Stats::characterDisplayStats, {Stats::fromElement(character.loadout.character.base.element)}};
							Team::Instance placeholderTeam{};
							placeholderTeam.stats.characters.at(0) = &character;
							Formula::Context ctx{
								.source = character.loadout,
								.target = character.loadout,
								.team = placeholderTeam.stats,
								.enemy = ::Store::enemies.at(0).stats,
							};

							for (const auto &[stat, transparent]: std::views::zip(
									 std::views::join(statsToDisplay),
									 Utils::trueFalse
								 )) {
								auto formula = Formula::Stat{stat};
								w.addChild(UI::StatDisplay{
									.isTransparent = transparent,
									.stat = StatValue{.stat = stat, .value = formula.eval(ctx)},
								});
							}
						},
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
							.onClick = [characterKey = characterKey](GestureDetector::Event event) {
								auto &character = ::Store::characters.at(characterKey);
								event.widget.addOverlay(UI::CharacterEditor{
									.character = character,
									.onSubmit = [](const Character::Instance &character) {
										auto &instance = Store::characters.at(character.instanceKey);
										instance.loadout.character.sheet = character.loadout.character.sheet;
										instance.updateEvent.notify();
									},
								});
							},
						},
						Button{
							.text = "Delete",
							.style = ButtonStyle::Standard(),
							.onClick = [characterKey = characterKey](GestureDetector::Event) {
								auto &character = Store::characters.at(characterKey);
								character.loadout.artifact.equipped.unequipAll();
								Store::characters.erase(characterKey);
								Store::characterListUpdateEvent.notify();
							},
						},
					},
				}
			},
		};
	}
};

UI::CharacterCard::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Card{
		.widget = widget.withDefaultPadding(1.f),
		.child = Rebuilder{
			.rebuildEvent = ::Store::characters.at(characterKey).updateEvent,
			.buildFunc = [characterKey = characterKey, controller = controller]() {
				return Contents{
					.characterKey = characterKey,
					.controller = controller,
				};
			},
		},
	};
}
