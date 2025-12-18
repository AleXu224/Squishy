#include "weaponSelector.hpp"

#include "misc/weaponType.hpp"
#include "observer.hpp"

#include "misc/rarityToColor.hpp"
#include "weapon/weapons.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/fontIcon.hpp"
#include "widgets/grid.hpp"
#include "widgets/image.hpp"
#include "widgets/liteFilter.hpp"


#include "ranges"
#include "widgets/modal.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/stack.hpp"

using namespace squi;

struct WeaponSelectorWeaponCard : StatelessWidget {
	// Args
	Key key;
	Args widget{};
	const Weapon::Data &weapon;
	VoidObservable closeEvent;
	std::function<void(Weapon::DataKey)> notifySelection;

	[[nodiscard]] Child build(const Element &) const {
		auto stars = Row{
			.widget{
				.width = Size::Shrink,
				.height = Size::Shrink,
			},
			.children = [&]() {
				Children ret;
				for (int i = 0; i < weapon.baseStats.rarity; ++i) {
					ret.emplace_back(FontIcon{
						.size = 16.f,
						.font = FontStore::defaultIconsFilled,
						.color = Color::orange,
						.icon = 0xE838,
					});
				}
				return ret;
			}(),
		};

		auto details = Column{
			.widget{
				.height = Size::Shrink,
				.padding = Padding(0.f, 8.f).withRight(8.f),
			},
			.spacing = 12.f,
			.children{
				Text{
					.text = weapon.name,
					.fontSize = 14.f,
					.lineWrap = true,
				},
				stars,
			},
		};

		auto image = Box{
			.widget{
				.width = 96.f,
				.height = 96.f,
			},
			.color = Misc::rarityToColor.at(weapon.baseStats.rarity),
			.borderRadius = BorderRadius::Left(4.f),
			.child = Image{
				.fit = squi::Image::Fit::contain,
				.image = ImageProvider::fromFile(std::format("assets/Weapons/{}/icon_ascended.png", weapon.name)),
			},
		};

		auto content = Row{
			.spacing = 12.f,
			.children{
				image,
				details,
			},
		};

		return Button{
			.widget{
				.width = Size::Expand,
				.height = Size::Shrink,
				.padding = 0.f,
			},
			.theme = Button::Theme::Standard(),
			.onClick = [this]() {
				if (notifySelection) notifySelection(weapon.key);
				closeEvent.notify();
			},
			.child = content,
		};
	}
};

squi::core::Child UI::WeaponSelector::State::build(const Element &element) {
	if (widget->type.has_value()) {
		for (const auto &weaponType: Misc::weaponTypes) {
			weaponTypes[weaponType] = false;
		}
		weaponTypes[widget->type.value()] = true;
	} else {
		for (const auto &weaponType: Misc::weaponTypes) {
			weaponTypes[weaponType] = true;
		}
	}


	auto typeFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret{};
			ret.reserve(Misc::weaponTypes.size());

			for (const auto &weaponType: Misc::weaponTypes) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(weaponType),
					.onUpdate = [this, weaponType](bool active) {
						auto &status = weaponTypes.at(weaponType);
						if (status != active) {
							setState([&]() {
								status = active;
							});
						}
					},
				});
			}

			return ret;
		}(),
	};

	auto content = ScrollView{
		.children{
			Grid{
				.widget{
					.height = Size::Shrink,
					.margin = Margin(24.f, 0.f),
				},
				.columnCount = Grid::MinSize{200.f},
				.spacing = 8.f,
				.children = [this]() {
					Children ret{};
					for (const auto &[_, weapon]: Weapon::list | std::views::filter([&](auto &&iter) {
													  const auto &[_, weapon] = iter;
													  return weaponTypes.at(weapon.baseStats.type);
												  })) {
						ret.emplace_back(WeaponSelectorWeaponCard{.weapon = weapon, .closeEvent = closeEvent, .notifySelection = widget->onSelect});
					}
					return ret;
				}(),
			},
		},
	};

	auto header = Column{
		.widget{
			.height = Size::Shrink,
			.margin = Margin(24.f, 0.f),
		},
		.spacing = 16.f,
		.children{
			Stack{
				.children{
					Text{
						.widget{
							.alignment = Alignment::CenterLeft,
						},
						.text = "Select weapon",
						.fontSize = 28.f,
						.font = FontStore::defaultFontBold,
					},
					// Align{
					// 	.xAlign = 1.f,
					// 	// FIXME: implement searching
					// 	// .child = TextBox{},
					// },
				},
			},
			widget->type.has_value() ? nullptr : Child(typeFilter),
		},
	};

	return Modal{
		.widget = widget->widget,
		.closeEvent = closeEvent,
		.child = Box{
			.widget{
				.height = Size::Shrink,
				.alignment = Alignment::Center,
				.sizeConstraints = BoxConstraints{.maxWidth = 800.f},
			},
			.color = Color::css(0x2C2C2C),
			.borderColor = Color::css(117, 117, 117, 0.4f),
			.borderWidth = 1.f,
			.borderRadius = 8.f,
			.borderPosition = squi::Box::BorderPosition::outset,
			.child = Column{
				.widget{
					.padding = Padding(0.f, 24.f),
				},
				.spacing = 24.f,
				.children{
					header,
					content
				},
			},
		},
	};
}
