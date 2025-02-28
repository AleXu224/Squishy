#include "characterCardBanner.hpp"

#include "column.hpp"
#include "container.hpp"
#include "image.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "store.hpp"
#include "text.hpp"

using namespace squi;

UI::CharacterCardBanner::operator squi::Child() const {
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
