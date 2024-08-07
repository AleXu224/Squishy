#include "characterCard.hpp"

#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "character/characters.hpp"

#include "column.hpp"
#include "container.hpp"
#include "image.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "text.hpp"


using namespace squi;

struct CharacterCardBanner {
	// Args
	Character::Instance &character;

	struct Storage {
		// Data
	};

	operator squi::Child() const {
		auto storage = std::make_shared<Storage>();

		return Stack{
			.widget{
				.height = 64.f,
			},
			.children{
				Container{
					.child = Image{
						.fit = squi::Image::Fit::cover,
						.image = Image::Data::fromFileAsync(std::format("assets/Characters/{}/banner.png", Character::list.at(character.key).name)),
					},
				},
				Row{
					.children{
						Image{
							.fit = squi::Image::Fit::contain,
							.image = Image::Data::fromFileAsync(std::format("assets/Characters/{}/avatar.png", Character::list.at(character.key).name)),
						},
						Column{
							.widget{
								.padding = Padding{4.f},
							},
							.spacing = 4.f,
							.children{
								Text{
									.text = Character::list.at(character.key).name,
									.fontSize = 24.f,
								},
								Text{
									.text = std::format("Lvl {}", character.stats.character.sheet.level),
								},
							},
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
		},
		.child = Column{
			.children{
				CharacterCardBanner{
					.character = character,
				},
				Column{
					.widget{
						.onInit = [&character = character](Widget &w) {
							auto statsToDisplay = {Stats::characterDisplayStats, {Stats::fromElement(character.stats.character.base.element)}};

							for (const auto &[stat, transparent]: std::views::zip(
									 std::views::join(statsToDisplay),
									 Utils::trueFalse
								 )) {
								auto val = character.stats.character.sheet.fromStat(stat).getTotal(character.stats);
								w.addChild(StatDisplay{
									.isTransparent = transparent,
									.stat = StatValue{.stat = stat, .value = val},
								});
							}
						},
					},
				},
			},
		},
	};
}
