#include "weaponSelector.hpp"

#include "Ui/utils/grid.hpp"
#include "align.hpp"
#include "box.hpp"
#include "button.hpp"
#include "column.hpp"
#include "fontIcon.hpp"
#include "image.hpp"
#include "misc/weaponType.hpp"
#include "modal.hpp"
#include "observer.hpp"

#include "misc/rarityToColor.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "stack.hpp"
#include "text.hpp"
#include "weapon/weapons.hpp"
#include "widgets/liteFilter.hpp"


#include "ranges"

using namespace squi;

struct WeaponSelectorWeaponCard {
	// Args
	squi::Widget::Args widget{};
	const Weapon::Data &weapon;
	VoidObservable closeEvent;
	std::function<void(Weapon::DataKey)> notifySelection;

	operator squi::Child() const {
		auto stars = Row{
			.widget{
				.width = Size::Shrink,
				.height = Size::Shrink,
			},
			.children = childrenFactory(weapon.baseStats.rarity, FontIcon{.icon = 0xE838, .font = FontStore::defaultIconsFilled, .size = 16.f, .color = Color::orange}),
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
				Align{
					.xAlign = 0.f,
					.child = details,
				},
			},
		};

		return Button{
			.widget{
				.width = Size::Expand,
				.height = Size::Shrink,
				.padding = 0.f,
			},
			.style = ButtonStyle::Standard(),
			.onClick = [notifySelection = notifySelection, &weapon = weapon, closeEvent = closeEvent](GestureDetector::Event) {
				if (notifySelection) notifySelection(weapon.key);
				closeEvent.notify();
			},
			.child = content,
		};
	}
};

UI::WeaponSelector::operator Child() const {
	auto storage = std::make_shared<Storage>();
	if (type.has_value()) {
		for (const auto &weaponType: Misc::weaponTypes) {
			storage->weaponTypes[weaponType] = false;
		}
		storage->weaponTypes[type.value()] = true;
	} else {
		for (const auto &weaponType: Misc::weaponTypes) {
			storage->weaponTypes[weaponType] = true;
		}
	}

	VoidObservable closeEvent{};
	VoidObservable filterUpdateEvent{};

	auto typeFilter = LiteFilter{
		.multiSelect = true,
		.items = [&]() {
			std::vector<LiteFilter::Item> ret{};
			ret.reserve(Misc::weaponTypes.size());

			for (const auto &weaponType: Misc::weaponTypes) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(weaponType),
					.onUpdate = [weaponType, storage, filterUpdateEvent](bool active) {
						auto &status = storage->weaponTypes.at(weaponType);
						if (status != active) {
							status = active;
							filterUpdateEvent.notify();
						}
					},
				});
			}

			return ret;
		}(),
	};

	auto makeWeaponList = [storage, closeEvent = closeEvent, onSelect = onSelect]() {
		Children ret{};

		for (const auto &[_, weapon]: Weapon::list | std::views::filter([&](auto &&iter) {
										  const auto &[_, weapon] = iter;
										  return storage->weaponTypes.at(weapon.baseStats.type);
									  })) {
			ret.emplace_back(WeaponSelectorWeaponCard{.weapon = weapon, .closeEvent = closeEvent, .notifySelection = onSelect});
		}

		return ret;
	};
	auto content = ScrollableFrame{
		.children{
			Grid{
				.widget{
					.height = Size::Shrink,
					.margin = Margin(24.f, 0.f),
					.onInit = [filterUpdateEvent, makeWeaponList](Widget &w) {
						observe(w, filterUpdateEvent, [&w, makeWeaponList]() {
							w.setChildren(makeWeaponList());
						});
					},
				},
				.spacing = 8.f,
				.columnCount = Grid::MinSize{200.f},
				.children = makeWeaponList(),
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
					Align{
						.xAlign = 0.f,
						.child = Text{.text = "Select weapon", .fontSize = 28.f, .font = FontStore::defaultFontBold},
					},
					Align{
						.xAlign = 1.f,
						// FIXME: implement searching
						// .child = TextBox{},
					},
				},
			},
			type.has_value() ? nullptr : Child(typeFilter),
		},
	};

	return Modal{
		.widget = widget,
		.closeEvent = closeEvent,
		.child = Box{
			.widget{
				.height = Size::Shrink,
				.sizeConstraints{.maxWidth = 800.f},
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