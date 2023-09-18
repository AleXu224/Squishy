#include "UI/Screens/CharacterScreen.hpp"
#include "UI/BackDrawer.hpp"
#include "UI/Card.hpp"
#include "UI/CharacterCard.hpp"
#include "UI/Grid.hpp"
#include "UI/Screens/CharacterDetails.hpp"
#include "UI/Screens/CharacterSettings.hpp"
#include "align.hpp"
#include "box.hpp"
#include "characterStore.hpp"
#include "column.hpp"
#include "container.hpp"
#include "expander.hpp"
#include "gestureDetector.hpp"
#include "navigationView.hpp"
#include "observer.hpp"
#include "scrollableFrame.hpp"
#include "text.hpp"
#include "widget.hpp"
#include <debugapi.h>
#include <memory>
#include <print>



using namespace squi;
using namespace Squishy;

struct CharacterSummary {
	// Args
	Widget::Args widget;
	std::shared_ptr<Character> character;
	std::shared_ptr<CharacterScreen::State> state;

	struct Storage {
		// Data
		std::shared_ptr<Character> character;
		std::shared_ptr<squi::VoidObservable> closeEvent = squi::VoidObservable::create();
		std::shared_ptr<VoidObservable> statsChangedEvent = VoidObservable::create();
		std::shared_ptr<squi::VoidObservable::Observer> closeObserver;
	};

	operator Child() const {
		auto storage = std::make_shared<Storage>(character);

		return Card{
			.widget = widget,
			.child = Column{
				.children{
					GestureDetector{
						.onClick = [state = state, storage](auto) {
							Child child = NavigationView{
								.widget{
									.onInit = [storage](Widget &widget) {
										storage->closeObserver = storage->closeEvent->observe([w = widget.weak_from_this()]() {
											if (auto widget = w.lock())
												widget->deleteLater();
										});
									},
								},
								.expanded = false,
								.backAction = [storage]() {
									storage->closeEvent->notify();
								},
								.pages{
									NavigationView::Page{
										.name = "Summary",
										.icon = 0xEE92,
										.content = CharacterDetails{
											.character = storage->character,
											.statsChangedEvent = storage->statsChangedEvent,
										},
									},
									NavigationView::Page{
										.name = "Character Setup",
										.icon = 0xEF58,
										.content = CharacterSettings{
											.statsChangedEvent = storage->statsChangedEvent,
											.character = storage->character,
										},
									},
									NavigationView::Page{
										.name = "Optimize",
										.icon = 0xF272,
									},
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