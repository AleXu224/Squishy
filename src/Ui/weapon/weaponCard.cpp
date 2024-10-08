#include "weaponCard.hpp"
#include "Ui/rarityToColor.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "image.hpp"
#include "stack.hpp"
#include "store.hpp"
#include "text.hpp"


using namespace squi;

struct WeaponHeader {
	// Args
	Weapon::Instance &weapon;

	struct Storage {
		// Data
	};

	operator squi::Child() const {
		auto storage = std::make_shared<Storage>();

		return Box{
			.widget{
				.height = 128.f,
			},
			.color = Utils::rarityToColor.at(Rarity::fiveStar),
			.borderRadius{7.f, 7.f, 0.f, 0.f},
			.child = Stack{
				.children{
					Align{
						.xAlign = 1.f,
						.child = Image{
							.fit = squi::Image::Fit::contain,
							.image = ImageProvider::fromFile(std::format("assets/Weapons/{}/icon_ascended.png", weapon.data.name)),
						},
					},
					Align{
						.xAlign = 0.f,
						.child = Column{
							.widget{
								.height = Size::Shrink,
								.padding = Padding{12.f, 0.f},
							},
							.spacing = 4.f,
							.children{
								Text{
									.text = weapon.data.name,
									.fontSize = 16.f,
									.lineWrap = true,
								},
								Text{
									.text = std::format("Lvl {}", weapon.stats.sheet.level),
									.fontSize = 20.f,
								},
								Text{
									.text = std::format("Refinement {}", weapon.stats.sheet.refinement),
									.fontSize = 20.f,
								},
							},
						},
					},
				},
			},
		};
	}
};

UI::WeaponCard::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Card{
		.widget{
			.padding = Padding{1.f},
		},
		.child = Column{
			.children{
				WeaponHeader{
					.weapon = weapon,
				},
				Column{
					.widget{
						.padding = Padding{4.f},
						.onInit = [instanceKey = weapon.instanceKey](Widget &w) {
							auto &weapon = ::Store::weapons.at(instanceKey);
							w.addChild(StatDisplay{
								.isTransparent = false,
								.stat{
									.stat = Stat::atk,
									.value = weapon.stats.base.getAtkAt(weapon.stats.sheet.level, weapon.stats.sheet.ascension),
								},
							});
							w.addChild(StatDisplay{
								.isTransparent = true,
								.stat{
									.stat = weapon.data.baseStats.substat.stat,
									.value = weapon.stats.base.getSubstatAt(weapon.stats.sheet.level),
								},
							});
						},
					},
				},
			},
		}
	};
}
