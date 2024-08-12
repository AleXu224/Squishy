#include "toggleConditional.hpp"
#include "button.hpp"
#include "fontIcon.hpp"
#include "observer.hpp"
#include "row.hpp"
#include "store.hpp"

using namespace squi;

struct ToggleBox {
	// Args
	squi::Widget::Args widget{};
	Observable<bool> switchEvent;
	CountObserver readyEvent;

	struct Storage {
		// Data
	};

	operator squi::Child() const {
		auto storage = std::make_shared<Storage>();

		return Box{
			.widget{
				.width = 20.f,
				.height = 20.f,
				.onInit = [switchEvent = switchEvent, readyEvent = readyEvent](Widget &w) {
					w.customState.add(switchEvent.observe([&w](bool active) {
						auto &box = w.as<Box::Impl>();
						if (active) {
							box.setBorderWidth(glm::vec4{0.f});
							box.setColor(Color{0x60CDFFFF});
						} else {
							box.setBorderWidth(glm::vec4{1.f});
							box.setColor(Color{0.f, 0.f, 0.f, 0.1f});
						}
					}));
					readyEvent.notify();
				},
			},
			.borderColor{1.f, 1.f, 1.f, 0.60},
			.borderRadius{4.f},
			.borderPosition = squi::Box::BorderPosition::outset,
			.child = Align{
				.child = FontIcon{
					.widget{
						.onInit = [switchEvent = switchEvent, readyEvent = readyEvent](Widget &w) {
							w.customState.add(switchEvent.observe([&w](bool active) {
								w.flags.visible = active;
							}));
							readyEvent.notify();
						},
					},
					.icon = 0xE73E,
					.color{0.f, 0.f, 0.f, 1.0f},
				},
			},
		};
	}
};

UI::ToggleConditional::operator squi::Child() const {
	auto storage = std::make_shared<Storage>(Storage{
		.active = conditional.active,
	});
	Observable<bool> switchEvent{};
	CountObserver readyEvent{2};

	return Button{
		.widget{
			.width = Size::Expand,
			.onInit = [readyEvent, switchEvent, storage](Widget &w) {
				w.customState.add(readyEvent.observe([storage, switchEvent]() {
					switchEvent.notify(storage->active);
				}));
			},
		},
		.style = ButtonStyle::Subtle(),
		.onClick = [storage, switchEvent, &conditional = conditional, characterKey = characterKey](GestureDetector::Event) {
			storage->active = !storage->active;
			conditional.active = storage->active;
			switchEvent.notify(storage->active);
			Store::characters.at(characterKey).updateEvent.notify();
		},
		.child = Row{
			.alignment = squi::Row::Alignment::center,
			.spacing = 8.f,
			.children{
				ToggleBox{
					.switchEvent = switchEvent,
					.readyEvent = readyEvent,
				},
				Align{
					.xAlign = 0.f,
					.child = Text{
						.text = conditional.name,
					},
				},
			},
		},
	};
}
