#include "weaponCard.hpp"
#include "Ui/rarityToColor.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/weapon/weaponEditor.hpp"
#include "align.hpp"
#include "box.hpp"
#include "button.hpp"
#include "column.hpp"
#include "image.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "store.hpp"
#include "text.hpp"
#include "weapon/data.hpp"


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
							.image = ImageProvider::fromFile(std::format("assets/Weapons/{}/icon_ascended.png", weapon.stats.data->name)),
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
									.text = weapon.stats.data->name,
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

struct WeaponCardContent {
	// Args
	squi::Widget::Args widget{};
	Weapon::Instance &weapon;

	operator squi::Child() const {
		auto header = WeaponHeader{
			.weapon = weapon,
		};
		auto content = Column{
			.widget{
				.padding = Padding{4.f},
				.onInit = [instanceKey = weapon.instanceKey](Widget &w) {
					auto &weapon = ::Store::weapons.at(instanceKey);
					w.addChild(UI::StatDisplay{
						.isTransparent = false,
						.stat{
							.stat = Stat::atk,
							.value = weapon.stats.data->baseStats.getAtkAt(weapon.stats.sheet.level, weapon.stats.sheet.ascension),
						},
					});
					w.addChild(UI::StatDisplay{
						.isTransparent = true,
						.stat{
							.stat = weapon.stats.data->baseStats.substat.stat,
							.value = weapon.stats.data->baseStats.getSubstatAt(weapon.stats.sheet.level),
						},
					});
				},
			},
		};
		auto footer = Row{
			.widget{
				.height = Size::Shrink,
				.padding = 4.f,
			},
			.spacing = 4.f,
			.children{
				Button{
					.text = "Edit",
					.style = ButtonStyle::Standard(),
					.onClick = [weapon = weapon](GestureDetector::Event event) {
						event.widget.addOverlay(UI::WeaponEditor{
							.weapon = weapon,
							.onSubmit = [](const Weapon::Instance &weapon) {
								auto &instance = Store::weapons.at(weapon.instanceKey);
								instance.stats = weapon.stats;
								instance.updateEvent.notify();
								for (const auto &[_, character]: Store::characters) {
									if (character.weaponInstanceKey == weapon.instanceKey) {
										character.updateEvent.notify();
									}
								}
							},
						});
					},
				},
				Button{
					.text = "Delete",
					.style = ButtonStyle::Standard(),
					.onClick = [key = weapon.instanceKey](GestureDetector::Event) {
						for (const auto &[_, instance]: Store::characters) {
							if (instance.weaponInstanceKey == key) {
								std::println("Cannot delete a used weapon");
								return;
							}
						}

						Store::weapons.erase(key);
						Store::weaponListUpdateEvent.notify();
					},
				},
			},
		};
		return Column{
			.children{
				header,
				content,
				footer,
			},
		};
	}
};

UI::WeaponCard::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Card{
		.widget{
			.padding = Padding{1.f},
			.onInit = [updateEvent = weapon.updateEvent, key = weapon.instanceKey](Widget &w) {
				w.customState.add(updateEvent.observe([&w, key]() {
					if (!Store::weapons.contains(key)) {
						w.deleteLater();
						return;
					}
					w.setChildren({
						WeaponCardContent{.weapon = Store::weapons.at(key)},
					});
				}));
			},
		},
		.child = WeaponCardContent{.weapon = weapon},
	};
}
