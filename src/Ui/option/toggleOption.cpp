#include "toggleOption.hpp"
#include "Ui/utils/decodeModsSheet.hpp"
#include "align.hpp"
#include "button.hpp"
#include "column.hpp"
#include "container.hpp"
#include "fontIcon.hpp"
#include "observer.hpp"
#include "row.hpp"
#include "store.hpp"
#include "text.hpp"

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
							box.setBorderWidth(0.f);
							box.setColor(Color{0x60CDFFFF});
						} else {
							box.setBorderWidth(1.f);
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
					.icon = 0xe5ca,
					.color{0.f, 0.f, 0.f, 1.0f},
				},
			},
		};
	}
};

UI::ToggleOption::operator squi::Child() const {
	auto storage = std::make_shared<Storage>(Storage{
		.active = option.active,
	});
	Observable<bool> switchEvent{};
	CountObserver readyEvent{2};

	auto mods = decodeOption(option, ctx);
	auto hasMods = !mods.empty();

	return Box{
		.color = hasMods ? Color::css(0x0, 0.3f) : Color::transparent,
		.borderRadius = 4.f,
		.child = Column{
			.children{
				Button{
					.widget{
						.width = Size::Expand,
						.onInit = [readyEvent, switchEvent, storage](Widget &w) {
							w.customState.add(readyEvent.observe([storage, switchEvent]() {
								switchEvent.notify(storage->active);
							}));
						},
					},
					.style = ButtonStyle::Subtle(),
					.onClick = [storage, switchEvent, &option = option, instanceKey = instanceKey](GestureDetector::Event) {
						storage->active = !storage->active;
						option.active = storage->active;
						switchEvent.notify(storage->active);
						std::visit(
							Utils::overloaded{
								[](const Character::InstanceKey &key) {
									::Store::characters.at(key).updateEvent.notify();
								},
								[](const Team::InstanceKey &key) {
									auto &team = ::Store::teams.at(key);
									for (const auto &character: team.stats.characters) {
										if (!character) continue;
										character->updateEvent.notify();
									}
								},
							},
							instanceKey
						);
					},
					.child = Row{
						.alignment = squi::Row::Alignment::center,
						.spacing = 8.f,
						.children{
							ToggleBox{
								.switchEvent = switchEvent,
								.readyEvent = readyEvent,
							},
							Container{
								.child = Text{
									.text = option.name,
									.lineWrap = true,
								},
							},
						},
					},
				},
				Column{
					.widget{
						.padding = 4.f,
						.onInit = [hasMods](Widget &w) {
							w.flags.visible = hasMods;
						},
					},
					.children = mods,
				},
			},
		},
	};
}
