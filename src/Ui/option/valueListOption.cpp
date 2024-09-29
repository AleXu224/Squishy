#include "valueListOption.hpp"

#include "align.hpp"
#include "button.hpp"
#include "container.hpp"
#include "contextMenu.hpp"
#include "fontIcon.hpp"
#include "row.hpp"
#include "store.hpp"
#include "text.hpp"

#include "ranges"
#include "vector"

using namespace squi;

UI::ValueListOption::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();
	CountObserver readyEvent(2);
	Observable<std::optional<uint32_t>> valueChangedEvent{};

	auto buttonText = Text{
		.widget{
			.onInit = [readyEvent, valueChangedEvent, &option = option](Widget &w) {
				w.customState.add(valueChangedEvent.observe([&w, &option](std::optional<uint32_t> newValue) {
					auto &text = w.as<Text::Impl>();
					if (newValue.has_value()) {
						text.setText(std::format("{}: {}", option.prefix, newValue.value()));
						text.setColor(Color{0.f, 0.f, 0.f, 0.9f});
					} else {
						text.setText(std::format("{}: Not Active", option.prefix));
						text.setColor(Color{1.f, 1.f, 1.f, 0.9f});
					}
				}));
				readyEvent.notify();
			},
		},
		.text = "placeholder",
		.lineWrap = true,
	};

	auto buttonCaret = FontIcon{
		.textWidget{
			.onInit = [readyEvent, valueChangedEvent](Widget &w) {
				w.customState.add(valueChangedEvent.observe([&w](std::optional<uint32_t> newValue) {
					auto &text = w.as<Text::Impl>();
					if (newValue.has_value()) {
						text.setColor(Color{0.f, 0.f, 0.f, 0.9f});
					} else {
						text.setColor(Color{1.f, 1.f, 1.f, 0.9f});
					}
				}));
				readyEvent.notify();
			},
		},
		.icon = 0xE972,
	};

	auto buttonContent = Row{
		.alignment = squi::Row::Alignment::center,
		.children{
			Container{
				.child = Align{
					.xAlign = 0.f,
					.child = buttonText,
				},
			},
			buttonCaret,
		},
	};

	return Button{
		.widget{
			.width = Size::Expand,
			.height = Size::Shrink,
			.sizeConstraints{
				.minHeight = 32.f,
			},
			.onInit = [readyEvent, valueChangedEvent, &option = option](Widget &w) {
				w.customState.add(valueChangedEvent.observe([&w](std::optional<uint32_t> newVal) {
					if (newVal.has_value()) {
						w.customState.get<ButtonStyle>("style") = ButtonStyle::Accent();
					} else {
						w.customState.get<ButtonStyle>("style") = ButtonStyle::Standard();
					}
				}));
				w.customState.add(readyEvent.observe([valueChangedEvent, &option]() {
					valueChangedEvent.notify(option.getValue());
				}));
			},
		},
		.style = option.getValue().has_value() ? ButtonStyle::Accent() : ButtonStyle::Standard(),
		.onClick = [valueChangedEvent, &option = option, characterKey = characterKey](GestureDetector::Event event) {
			event.widget.addOverlay(ContextMenu{
				.position = event.widget.getPos().withYOffset(event.widget.getSize().y),
				.items = [&]() {
					std::vector<ContextMenu::Item> ret{
						ContextMenu::Item{
							.text = "Not Active",
							.content = [valueChangedEvent, &option, characterKey]() {
								option.currentIndex = std::nullopt;
								valueChangedEvent.notify(option.getValue());
								::Store::characters.at(characterKey).updateEvent.notify();
							},
						},
						ContextMenu::Item{
							.text{},
							.content = ContextMenu::Item::Divider{},
						},
					};

					for (const auto &[index, item]: std::views::enumerate(option.values)) {
						ret.emplace_back(ContextMenu::Item{
							.text = std::format("{}", item),
							.content = [index, valueChangedEvent, &option, characterKey]() {
								option.currentIndex = index;
								valueChangedEvent.notify(option.getValue());
								::Store::characters.at(characterKey).updateEvent.notify();
							},
						});
					}

					return ret;
				}(),
			});
		},
		.child = buttonContent,
	};
}
