#include "characterCard.hpp"

#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "character/characters.hpp"
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
									.text = std::format("Lvl {}", character.loadout.character.sheet.level),
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

	operator squi::Child() const {
		return Column{
			.children{
				CharacterCardBanner{
					.characterKey = characterKey,
				},
				Column{
					.widget{
						.padding = Padding{4.f},
						.onInit = [characterKey = characterKey](Widget &w) {
							auto &character = ::Store::characters.at(characterKey);
							auto statsToDisplay = std::vector{Stats::characterDisplayStats, {Stats::fromElement(character.loadout.character.base.element)}};
							Team::Instance placeholderTeam{};
							placeholderTeam.stats.characters.at(0) = std::ref(character);
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
								auto val = character.loadout.character.sheet.postMods.fromStat(stat).get(ctx);
								w.addChild(UI::StatDisplay{
									.isTransparent = transparent,
									.stat = StatValue{.stat = stat, .value = val},
								});
							}
						},
					},
				},
			},
		};
	}
};

UI::CharacterCard::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Card{
		.widget{
			.padding = Padding{1.f},
			.onInit = [characterKey = characterKey](Widget &w) {
				w.customState.add(::Store::characters.at(characterKey).updateEvent.observe([characterKey, &w]() {
					w.setChildren({Contents{.characterKey = characterKey}});
				}));
			},
		},
		.child = Contents{
			.characterKey = characterKey,
		},
	};
}
