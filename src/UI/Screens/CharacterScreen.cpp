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
#include "button.hpp"
#include "characterStore.hpp"
#include "column.hpp"
#include "container.hpp"
#include "gestureDetector.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "text.hpp"
#include "widget.hpp"
#include <debugapi.h>
#include <memory>
#include <vector>

using namespace squi;
using namespace Squishy;

struct CharacterDetailsContent {
	// Args
	Widget::Args widget;
	Character &character;

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
								.sheet = character.sheet,
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character.nodes.normal,
								.sheet = character.sheet,
								.name = "Normal Attack",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character.nodes.charged,
								.sheet = character.sheet,
								.name = "Charged Attack",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character.nodes.plunge,
								.sheet = character.sheet,
								.name = "Plunge Attack",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character.nodes.skill,
								.sheet = character.sheet,
								.name = "Skill",
							},
							NodeCard{
								.widget{
									.height = Size::Shrink,
								},
								.nodes = character.nodes.burst,
								.sheet = character.sheet,
								.name = "Burst",
							},
						},
					},
				},
			},
		};
	}
};

struct CharacterDetails {
	// Args
	Widget::Args widget;
	Character &character;

	struct Storage {
		// Data
		bool shouldClose = false;
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
						.widget{
							.padding = 8.f,
						},
						.spacing = 8.f,
						.children{
							Button{
								.text = "Back",
								.style = ButtonStyle::Standard(),
								// TODO INTRODUCE AUTO FOR PARAMETERS
								.onClick = [storage]() {
									storage->shouldClose = true;
								},
							},
							DropdownButton{},
						},
					},
				},
				CharacterDetailsContent{
					.character = character,
				},
			},
		};
	}
};

struct CharacterSummary {
	// Args
	Widget::Args widget;
	Character &character;
	std::shared_ptr<CharacterScreen::State> state;

	struct Storage {
		// Data
		Character &character;
	};

	operator Child() const {
		auto storage = std::make_shared<Storage>(character);

		return Card{
			.widget = widget,
			.child = Column{
				.children{
					GestureDetector{
						.onClick = [state = state, storage](auto &, auto &) {
							auto child = Child{
								CharacterDetails{
									.character = storage->character,
								}};
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
									.text = character.data.name,
									.fontSize = 24.f,
								},
							},
						},
					},
					StatsDisplay{
						.widget{
							.margin = 4.f,
						},
						.sheet = character.sheet,
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
								ret.reserve(CharacterStore::characters.size());
								for (auto &character: CharacterStore::characters) {
									ret.emplace_back(CharacterSummary{
										.widget{
											.height = Size::Shrink,
										},
										.character = *character,
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