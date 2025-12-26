#include "characterCardBanner.hpp"

#include "character/characters.hpp"
#include "store.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/image.hpp"
#include "widgets/row.hpp"
#include "widgets/stack.hpp"
#include "widgets/text.hpp"

using namespace squi;

[[nodiscard]] squi::core::Child UI::CharacterCardBanner::build(const Element &) const {
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
								.text = std::string(Character::list.at(character.dataKey).name),
								.fontSize = 24.f,
							},
							Text{
								.text = std::format("Lvl {} C{}", character.state.stats.sheet.level, character.state.stats.sheet.constellation),
							},
						},
					},
				},
			},
		},
	};
}
