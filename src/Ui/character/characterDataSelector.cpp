#include "characterDataSelector.hpp"

#include "misc/rarityToColor.hpp"
#include "ranges"

#include "character/characters.hpp"

#include "Ui/utils/grid.hpp"
#include "align.hpp"
#include "box.hpp"
#include "button.hpp"
#include "column.hpp"
#include "fontIcon.hpp"
#include "image.hpp"
#include "modal.hpp"
#include "observer.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "stack.hpp"
#include "text.hpp"
#include "widgets/liteFilter.hpp"


using namespace squi;

struct CharacterSelectorCharacterCard {
	// Args
	squi::Widget::Args widget{};
	const Character::Data &character;
	VoidObservable closeEvent;
	std::function<void(Character::DataKey)> notifySelection;

	operator squi::Child() const {
		auto stars = Row{
			.widget{
				.width = Size::Shrink,
				.height = Size::Shrink,
			},
			.children = childrenFactory(character.baseStats.rarity, FontIcon{.icon = 0xE838, .font = FontStore::defaultIconsFilled, .size = 16.f, .color = Color::orange}),
		};

		auto details = Column{
			.widget{
				.height = Size::Shrink,
				.padding = Padding(0.f, 8.f).withRight(8.f),
			},
			.spacing = 12.f,
			.children{
				Text{
					.text = character.name,
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
			.color = Misc::rarityToColor.at(character.baseStats.rarity),
			.borderRadius = BorderRadius::Left(4.f),
			.child = Image{
				.fit = squi::Image::Fit::contain,
				.image = ImageProvider::fromFile(std::format("assets/Characters/{}/avatar.png", character.name)),
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
			.onClick = [notifySelection = notifySelection, &character = character, closeEvent = closeEvent](GestureDetector::Event) {
				if (notifySelection) notifySelection(character.key);
				closeEvent.notify();
			},
			.child = content,
		};
	}
};

UI::CharacterDataSelector::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();
	for (const auto &element: Misc::characterElements) {
		storage->characterElements[element] = true;
	}
	for (const auto &weaponType: Misc::weaponTypes) {
		storage->characterWeapon[weaponType] = true;
	}

	VoidObservable closeEvent{};
	VoidObservable filterUpdateEvent{};

	auto elementFilter = LiteFilter{
		.multiSelect = true,
		.items = [&]() {
			std::vector<LiteFilter::Item> ret{};
			ret.reserve(Misc::characterElements.size());

			for (const auto &characterElement: Misc::characterElements) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(characterElement),
					.onUpdate = [characterElement, storage, filterUpdateEvent](bool active) {
						auto &status = storage->characterElements.at(characterElement);
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
	auto weaponTypeFilter = LiteFilter{
		.multiSelect = true,
		.items = [&]() {
			std::vector<LiteFilter::Item> ret{};
			ret.reserve(Misc::weaponTypes.size());

			for (const auto &weaponType: Misc::weaponTypes) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(weaponType),
					.onUpdate = [weaponType, storage, filterUpdateEvent](bool active) {
						auto &status = storage->characterWeapon.at(weaponType);
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

	auto makeCharacterList = [storage, closeEvent = closeEvent, onSelect = onSelect]() {
		Children ret{};

		for (const auto &[_, character]: Character::list | std::views::filter([&](auto &&iter) {
											 const auto &[_, character] = iter;
											 return storage->characterElements.at(character.baseStats.element) && storage->characterWeapon.at(character.baseStats.weaponType);
										 })) {
			ret.emplace_back(CharacterSelectorCharacterCard{.character = character, .closeEvent = closeEvent, .notifySelection = onSelect});
		}

		return ret;
	};
	auto content = ScrollableFrame{
		.children{
			Grid{
				.widget{
					.height = Size::Shrink,
					.margin = Margin(24.f, 0.f),
					.onInit = [filterUpdateEvent, makeCharacterList](Widget &w) {
						observe(w, filterUpdateEvent, [&w, makeCharacterList]() {
							w.setChildren(makeCharacterList());
						});
					},
				},
				.spacing = 8.f,
				.columnCount = Grid::MinSize{200.f},
				.children = makeCharacterList(),
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
						.child = Text{.text = "Select character", .fontSize = 28.f, .font = FontStore::defaultFontBold},
					},
					Align{
						.xAlign = 1.f,
						// FIXME: implement searching
						// .child = TextBox{},
					},
				},
			},
			Column{
				.widget{.height = Size::Shrink},
				.spacing = 8.f,
				.children{
					elementFilter,
					weaponTypeFilter,
				},
			},
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
