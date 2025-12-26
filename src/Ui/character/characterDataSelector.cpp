#include "characterDataSelector.hpp"

#include "misc/rarityToColor.hpp"
#include "ranges"

#include "character/characters.hpp"

#include "observer.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/fontIcon.hpp"
#include "widgets/grid.hpp"
#include "widgets/image.hpp"
#include "widgets/liteFilter.hpp"
#include "widgets/modal.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/stack.hpp"


using namespace squi;

struct CharacterDataSelectorCharacterCard : StatelessWidget {
	// Args
	Key key;
	Args widget{};
	const Character::Data &character;
	VoidObservable closeEvent;
	std::function<void(Character::DataKey)> notifySelection;

	[[nodiscard]] Child build(const Element &) const {
		Child stars = Row{
			.widget{
				.width = Size::Shrink,
				.height = Size::Shrink,
			},
			.children = [&]() {
				Children ret;
				for (int i = 0; i < character.baseStats.rarity; ++i) {
					ret.emplace_back(FontIcon{.size = 16.f, .font = FontStore::defaultIconsFilled, .color = Color::orange, .icon = 0xE838});
				}
				return ret;
			}(),
		};

		Child details = Column{
			.widget{
				.height = Size::Shrink,
				.padding = Padding(0.f, 8.f).withRight(8.f),
			},
			.spacing = 12.f,
			.children{
				Text{
					.text = std::string(character.name),
					.fontSize = 14.f,
					.lineWrap = true,
				},
				stars,
			},
		};

		Child image = Box{
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

		Child content = Row{
			.crossAxisAlignment = Row::Alignment::center,
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
			.onClick = [this]() {
				if (notifySelection) notifySelection(character.key);
				closeEvent.notify();
			},
			.child = content,
		};
	}
};

squi::core::Child UI::CharacterDataSelector::State::build(const Element &element) {
	auto elementFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret{};
			ret.reserve(Misc::characterElements.size());

			for (const auto &characterElement: Misc::characterElements) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(characterElement),
					.onUpdate = [characterElement, this](bool active) {
						auto &status = characterElements.at(characterElement);
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
	auto weaponTypeFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret{};
			ret.reserve(Misc::weaponTypes.size());

			for (const auto &weaponType: Misc::weaponTypes) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(weaponType),
					.onUpdate = [weaponType, this](bool active) {
						auto &status = characterWeapon.at(weaponType);
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

	auto makeCharacterList = [this]() {
		Children ret{};

		for (const auto &[_, character]: Character::list | std::views::filter([&](auto &&iter) {
											 const auto &[_, character] = iter;
											 return characterElements.at(character.baseStats.element) && characterWeapon.at(character.baseStats.weaponType);
										 })) {
			ret.emplace_back(CharacterDataSelectorCharacterCard{.character = character, .closeEvent = closeEvent, .notifySelection = widget->onSelect});
		}

		return ret;
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
					Text{
						.widget{
							.alignment = Alignment::CenterLeft,
						},
						.text = "Select character",
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
