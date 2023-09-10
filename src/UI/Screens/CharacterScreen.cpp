#include "UI/Screens/CharacterScreen.hpp"
#include "UI/BackDrawer.hpp"
#include "UI/Card.hpp"
#include "UI/CharacterCard.hpp"
#include "UI/DropdownButton.hpp"
#include "UI/Grid.hpp"
#include "UI/Masonry.hpp"
#include "UI/NodeCard.hpp"
#include "align.hpp"
#include "box.hpp"
#include "characterStore.hpp"
#include "column.hpp"
#include "container.hpp"
#include "fontIcon.hpp"
#include "gestureDetector.hpp"
#include "observer.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "talent.hpp"
#include "text.hpp"
#include "widget.hpp"
#include <debugapi.h>
#include <memory>
#include <print>
#include <string_view>

using namespace squi;
using namespace Squishy;

struct CharacterDetailsContent {
	// Args
	Widget::Args widget;
	std::shared_ptr<Character> character;
	VoidObservable &observable;

	operator Child() const {
		return ScrollableFrame{
			.children{
				Container{
					.widget{
						.height = Size::Shrink,
					},
					.child = Masonry{
						.widget{
							.padding = 12.f,
						},
						.spacing = 12.f,
						.minChildWidth = 300.f,
						.children{
							CharacterCard{
								.sheet = character->sheet,
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character->nodes.normal,
								.character = character,
								.conditionals = character->conditionals,
								.talent = Talent::Normal,
								.observable = observable,
								.name = "Normal Attack",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character->nodes.charged,
								.character = character,
								.conditionals = character->conditionals,
								.talent = Talent::Charged,
								.observable = observable,
								.name = "Charged Attack",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character->nodes.plunge,
								.character = character,
								.conditionals = character->conditionals,
								.talent = Talent::Plunge,
								.observable = observable,
								.name = "Plunge Attack",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character->nodes.skill,
								.character = character,
								.conditionals = character->conditionals,
								.talent = Talent::Skill,
								.observable = observable,
								.name = "Skill",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character->nodes.burst,
								.character = character,
								.conditionals = character->conditionals,
								.talent = Talent::Burst,
								.observable = observable,
								.name = "Burst",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character->nodes.passive1,
								.character = character,
								.conditionals = character->conditionals,
								.talent = Talent::Passive1,
								.observable = observable,
								.name = "Passive 1",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character->nodes.passive2,
								.character = character,
								.conditionals = character->conditionals,
								.talent = Talent::Passive2,
								.observable = observable,
								.name = "Passive 2",
							},
						},
					},
				},
			},
		};
	}
};

struct DropdownWithName {
	// Args
	Widget::Args widget;
	std::string_view text;
	Child child;

	operator Child() const {
		return Row{
			.widget{widget.withDefaultHeight(Size::Shrink).withDefaultMargin(Margin{12.f, 0.f})},
			.alignment = Row::Alignment::center,
			.children{
				Container{
					.widget{
						.height = Size::Shrink,
					},
					.child = Text{.text = text},
				},
				child,
			},
		};
	}
};

Children getDetailsChildren(VoidObservable &observable, std::shared_ptr<Character> character) {
	return {
		DropdownWithName{
			.text = "Level",
			.child = DropdownButton<uint8_t>{
				.value = character->sheet.level,
				.onSelect = [&observable, &character = character](auto item) {
					if (item.value > 80) character->sheet.ascension = 6;
					else if (item.value > 70)
						character->sheet.ascension = 5;
					else if (item.value > 60)
						character->sheet.ascension = 4;
					else if (item.value > 50)
						character->sheet.ascension = 3;
					else if (item.value > 40)
						character->sheet.ascension = 2;
					else if (item.value > 20)
						character->sheet.ascension = 1;
					else
						character->sheet.ascension = 0;
					character->update();
					observable.notify();
				},
				.items{
					{"1", 1},
					{"20", 20},
					{"40", 40},
					{"50", 50},
					{"60", 60},
					{"70", 70},
					{"80", 80},
					{"90", 90},
				},
			},
		},
		DropdownWithName{
			.text = "Ascension",
			.child = DropdownButton<uint8_t>{
				.value = character->sheet.ascension,
				.onSelect = [&observable, &character = character](auto item) {
					character->update();
					observable.notify();
				},
				.items{
					{"0", 0},
					{"1", 1},
					{"2", 2},
					{"3", 3},
					{"4", 4},
					{"5", 5},
					{"6", 6},
				},
			},
		},
		DropdownWithName{
			.text = "Constellation",
			.child = DropdownButton<uint8_t>{
				.value = character->sheet.constellation,
				.onSelect = [&observable, &character = character](auto item) {
					character->update();
					observable.notify();
				},
				.items = []() {
					std::vector<DropdownButton<uint8_t>::Item> ret{};
					for (uint8_t i = 0; i <= 6; ++i) {
						ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("C{}", i), i});
					}
					return ret;
				}(),
			},
		},
		DropdownWithName{
			.text = "Weapon Level",
			.child = DropdownButton<uint8_t>{
				.value = character->sheet.weaponLevel,
				.onSelect = [&observable, &character = character](auto item) {
					if (item.value > 80) character->sheet.weaponAscension = 6;
					else if (item.value > 70)
						character->sheet.weaponAscension = 5;
					else if (item.value > 60)
						character->sheet.weaponAscension = 4;
					else if (item.value > 50)
						character->sheet.weaponAscension = 3;
					else if (item.value > 40)
						character->sheet.weaponAscension = 2;
					else if (item.value > 20)
						character->sheet.weaponAscension = 1;
					else
						character->sheet.weaponAscension = 0;
					character->update();
					observable.notify();
				},
				.items{
					{"1", 1},
					{"20", 20},
					{"40", 40},
					{"50", 50},
					{"60", 60},
					{"70", 70},
					{"80", 80},
					{"90", 90},
				},
			},
		},
		DropdownWithName{
			.text = "Weapon Ascension",
			.child = DropdownButton<uint8_t>{
				.value = character->sheet.weaponAscension,
				.onSelect = [&observable, &character = character](auto item) {
					character->update();
					observable.notify();
				},
				.items{
					{"0", 0},
					{"1", 1},
					{"2", 2},
					{"3", 3},
					{"4", 4},
					{"5", 5},
					{"6", 6},
				},
			},
		},
		DropdownWithName{
			.text = "Weapon Refinement",
			.child = DropdownButton<uint8_t>{
				.value = character->sheet.weaponRefinement,
				.onSelect = [&observable, &character = character](auto item) {
					character->update();
					observable.notify();
				},
				.items = []() {
					std::vector<DropdownButton<uint8_t>::Item> ret{};
					for (uint8_t i = 1; i <= 5; ++i) {
						ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("R{}", i), i});
					}
					return ret;
				}(),
			},
		},
		DropdownWithName{
			.text = "Normal Attack Level",
			.child = DropdownButton<uint8_t>{
				.value = character->sheet.talents.normal,
				.onSelect = [&observable, &character = character](auto item) {
					character->update();
					observable.notify();
				},
				.items = []() {
					std::vector<DropdownButton<uint8_t>::Item> ret{};
					for (uint8_t i = 1; i <= 10; ++i) {
						ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("Level {}", i), static_cast<uint8_t>(i - 1)});
					}
					return ret;
				}(),
			},
		},
		DropdownWithName{
			.text = "Elemental Skill Level",
			.child = DropdownButton<uint8_t>{
				.value = character->sheet.talents.skill,
				.onSelect = [&observable, &character = character](auto item) {
					character->update();
					observable.notify();
				},
				.items = []() {
					std::vector<DropdownButton<uint8_t>::Item> ret{};
					for (uint8_t i = 1; i <= 10; ++i) {
						ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("Level {}", i), static_cast<uint8_t>(i - 1)});
					}
					return ret;
				}(),
			},
		},
		DropdownWithName{
			.text = "Elemental Burst Level",
			.child = DropdownButton<uint8_t>{
				.value = character->sheet.talents.burst,
				.onSelect = [&observable, &character = character](auto item) {
					character->update();
					observable.notify();
				},
				.items = []() {
					std::vector<DropdownButton<uint8_t>::Item> ret{};
					for (uint8_t i = 1; i <= 10; ++i) {
						ret.emplace_back(DropdownButton<uint8_t>::Item{std::format("Level {}", i), static_cast<uint8_t>(i - 1)});
					}
					return ret;
				}(),
			},
		},
	};
}

struct CharacterDetails {
	// Args
	Widget::Args widget;
	std::shared_ptr<Character> character;

	struct Storage {
		// Data
		bool shouldClose = false;
		VoidObservable observable{};
	};

	operator Child() const {
		auto storage = std::make_shared<Storage>();

		return Row{
			.widget{
				.afterUpdate = [storage](Widget &w) {
					if (storage->shouldClose) {
						w.deleteLater();
					}
				},
			},
			.children{
				Box{
					.widget{
						.width = 300.f,
						.margin = Margin{12.f, 0.f, 12.f, 12.f},
					},
					.color = Color::RGBA(1, 1, 1, 0.04),
					.borderRadius = 4.f,
					.child = ScrollableFrame{
						.scrollableWidget{
							.margin = 8.f,
						},
						.spacing = 8.f,
						.children = [&]() -> Children {
							Children ret{
								Button{
									// .text = "Back",
									.widget{
										.width = 40.f,
										.height = 36.f,
										.padding = 8.f,
									},
									.style = ButtonStyle::Subtle(),
									.onClick = [storage](auto) {
										storage->shouldClose = true;
									},
									.child = Align{
										.child = FontIcon{
											.icon = 0xF0D3,
											.size = 12.f,
										},
									},
								},
							};
							Children ret2 = getDetailsChildren(storage->observable, character);
							ret.insert(ret.end(), ret2.begin(), ret2.end());
							return ret;
						}(),
					},
				},
				CharacterDetailsContent{
					.character = character,
					.observable = storage->observable,
				},
			},
		};
	}
};

struct CharacterSummary {
	// Args
	Widget::Args widget;
	std::shared_ptr<Character> character;
	std::shared_ptr<CharacterScreen::State> state;

	struct Storage {
		// Data
		std::shared_ptr<Character> character;
	};

	operator Child() const {
		auto storage = std::make_shared<Storage>(character);

		return Card{
			.widget = widget,
			.child = Column{
				.children{
					GestureDetector{
						.onClick = [state = state, storage](auto) {
							auto child = Child{
								CharacterDetails{
									.character = storage->character,
								},
							};
							state->screenToOpen.swap(child);
						},
						.child = Box{
							.widget{
								.height = 64.f,
								.padding = Margin{0.f, 16.f, 0.f, 16.f},
							},
							.color = Color::RGBA(1, 1, 1, 0.08),
							.child = Align{
								.xAlign = 0.f,
								.child = Text{
									.text = character->data.name,
									.fontSize = 24.f,
								},
							},
						},
					},
					StatsDisplay{
						.widget{
							.margin = 4.f,
						},
						.sheet = character->sheet,
					},
				},
			},
		};
	}
};

CharacterScreen::operator Child() const {
	auto state = std::make_shared<State>();

	return BackDrawer{
		.widget{
			.onUpdate = [state](Widget &w) {
				if (state->screenToOpen) {
					w.addChild(state->screenToOpen);
					state->screenToOpen.reset();
				}
			},
		},
		.children{
			ScrollableFrame{
				.children{
					Container{
						.widget{
							.height = Size::Shrink,
							.margin = 12.f,
						},
						.child = Grid{
							.spacing = 12.f,
							.minChildWidth = 250.f,
							.children = [&]() -> Children {
								std::vector<Child> ret{};
								ret.reserve(Store::characters.size());
								for (auto &character: Store::characters) {
									ret.emplace_back(CharacterSummary{
										.widget{
											.height = Size::Shrink,
										},
										.character = character,
										.state = state,
									});
								}
								return ret;
							}(),
						},
					},
				},
			},
		},
	};
}