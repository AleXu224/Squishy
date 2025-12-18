#include "weaponCard.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/weapon/weaponEditor.hpp"
#include "misc/rarityToColor.hpp"
#include "store.hpp"
#include "weapon/data.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/image.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/stack.hpp"
#include "widgets/text.hpp"


using namespace squi;

struct WeaponHeader : StatelessWidget {
	// Args
	Key key;
	Weapon::Instance &weapon;

	[[nodiscard]] Child build(const Element &) const {
		return Box{
			.widget{
				.height = 137.f,
			},
			.color = Color::css(0xffffff, 0.0419f),
			.borderRadius{7.f, 7.f, 0.f, 0.f},
			.child = Stack{
				.children{
					Image{
						.widget{
							.alignment = Alignment::CenterRight,
						},
						.fit = squi::Image::Fit::contain,
						.image = ImageProvider::fromFile(std::format("assets/Weapons/{}/icon{}.png", weapon.stats.data->name, weapon.stats.sheet.ascension >= 2 ? "_ascended" : "")),
					},
					Column{
						.widget{
							.height = Size::Shrink,
							.alignment = Alignment::CenterRight,
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
							weapon.stats.data->baseStats.rarity <= 2//
								? Child()
								: Text{
									  .text = std::format("Refinement {}", weapon.stats.sheet.refinement),
									  .fontSize = 20.f,
								  },
						},
					},
				},
			},
		};
	}
};

struct WeaponCardContent : StatelessWidget {
	// Args
	Key key;
	Args widget{};
	Weapon::Instance &weapon;
	UI::WeaponCard::Actions actions;

	[[nodiscard]] Child build(const Element &element) const {
		auto header = WeaponHeader{
			.weapon = weapon,
		};
		auto content = Column{
			.widget{
				.padding = Padding{4.f},
			},
			.children{
				UI::StatDisplay{
					.isTransparent = false,
					.stat{
						.stat = Stat::atk,
						.value = weapon.stats.data->baseStats.getAtkAt(weapon.stats.sheet.level, weapon.stats.sheet.ascension),
					},
				},
				weapon.stats.data->baseStats.subStat.has_value()//
					? UI::StatDisplay{
						  .isTransparent = true,
						  .stat = {
							  .stat = weapon.stats.data->baseStats.subStat.value().stat.stat,
							  .value = weapon.stats.data->baseStats.getSubstatAt(weapon.stats.sheet.level),
						  },
					  }
					: Child{},
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
					.theme = Button::Theme::Standard(),
					.onClick = [&]() {
						Navigator::of(element).pushOverlay(UI::WeaponEditor{
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
					.child = "Edit",
				},
				actions == UI::WeaponCard::Actions::list//
					? Button{
						  .theme = Button::Theme::Standard(),
						  .onClick = [this]() {
							  if (!weapon.usedOn().empty()) {
								  std::println("Cannot delete a used weapon");
								  return;
							  }

							  Store::weapons.erase(weapon.instanceKey);
							  Store::weaponListUpdateEvent.notify();
						  },
						  .child = "Delete",
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

squi::core::Child UI::WeaponCard::State::build(const Element &element) {
	return Card{
		.widget{
			.padding = Padding{1.f},
		},
		.borderColor = Misc::rarityToColor.at(widget->weapon.stats.data->baseStats.rarity),
		.child = WeaponCardContent{
			.weapon = widget->weapon,
			.actions = widget->actions,
		},
	};
	;
}
