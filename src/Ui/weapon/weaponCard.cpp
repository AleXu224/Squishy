#include "weaponCard.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/weapon/weaponEditor.hpp"
#include "align.hpp"
#include "box.hpp"
#include "button.hpp"
#include "column.hpp"
#include "image.hpp"
#include "misc/rarityToColor.hpp"
#include "rebuilder.hpp"
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
				.height = 137.f,
			},
			.color = Color::css(0xffffff, 0.0419f),
			.borderRadius{7.f, 7.f, 0.f, 0.f},
			.child = Stack{
				.children{
					Align{
						.xAlign = 1.f,
						.child = Image{
							.fit = squi::Image::Fit::contain,
							.image = ImageProvider::fromFile(std::format("assets/Weapons/{}/icon{}.png", weapon.stats.data->name, weapon.stats.sheet.ascension >= 2 ? "_ascended" : "")),
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
								weapon.stats.data->baseStats.rarity <= 2 ? Child() : Text{
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
	UI::WeaponCard::Actions actions;

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
					if (weapon.stats.data->baseStats.subStat.has_value()) {
						w.addChild(UI::StatDisplay{
							.isTransparent = true,
							.stat{
								.stat = weapon.stats.data->baseStats.subStat.value().stat.stat,
								.value = weapon.stats.data->baseStats.getSubstatAt(weapon.stats.sheet.level),
							},
						});
					}
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
								for (auto &[_, character]: Store::characters) {
									if (!weapon.isUsedOn(character.instanceKey)) continue;
									instance.stats.data->getOpts(character.state.options);
									character.updateEvent.notify();
								}
							},
						});
					},
				},
				actions == UI::WeaponCard::Actions::list//
					? Button{
						  .text = "Delete",
						  .style = ButtonStyle::Standard(),
						  .onClick = [key = weapon.instanceKey](GestureDetector::Event) {
							  auto &weapon = Store::weapons.at(key);
							  if (!weapon.usedOn().empty()) {
								  std::println("Cannot delete a used weapon");
								  return;
							  }

							  Store::weapons.erase(key);
							  Store::weaponListUpdateEvent.notify();
						  },
					  }
					: Child{},
			},
		};
		return Column{
			.children{
				header,
				content,
				actions != UI::WeaponCard::Actions::showcase ? footer : Child{},
			},
		};
	}
};

UI::WeaponCard::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Rebuilder{
		.widget = widget,
		.rebuildEvent = weapon.updateEvent,
		.buildFunc = [key = weapon.instanceKey, actions = actions]() -> Child {
			if (!Store::weapons.contains(key)) {
				return Child{};
			}
			auto weapon = Store::weapons.at(key);
			return Card{
				.widget{
					.padding = Padding{1.f},
				},
				.borderColor = Misc::rarityToColor.at(weapon.stats.data->baseStats.rarity),
				.child = WeaponCardContent{
					.weapon = weapon,
					.actions = actions,
				},
			};
		},
	};
}
