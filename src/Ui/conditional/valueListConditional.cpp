#include "valueListConditional.hpp"
#include "button.hpp"

#include "container.hpp"
#include "contextMenu.hpp"
#include "fontIcon.hpp"
#include "row.hpp"
#include "window.hpp"

#include "vector"

using namespace squi;

UI::ValueListConditional::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();
	CountObserver readyEvent(2);
	Observable<std::optional<uint32_t>> valueChangedEvent{};

	return Button{
		.widget{
			.width = Size::Expand,
			.onInit = [readyEvent, valueChangedEvent, &conditional = conditional](Widget &w) {
				w.customState.add(valueChangedEvent.observe([&w](std::optional<uint32_t> newVal) {
					if (newVal.has_value()) {
						w.customState.get<ButtonStyle>("style") = ButtonStyle::Accent();
					} else {
						w.customState.get<ButtonStyle>("style") = ButtonStyle::Standard();
					}
				}));
				w.customState.add(readyEvent.observe([valueChangedEvent, &conditional]() {
					valueChangedEvent.notify(conditional.getValue());
				}));
			},
		},
		.style = ButtonStyle::Standard(),
		.onClick = [valueChangedEvent, &conditional = conditional](GestureDetector::Event event) {
			Window::of(&event.widget).addOverlay(ContextMenu{
				.position = event.widget.getPos().withYOffset(event.widget.getSize().y),
				.items = [&]() {
					std::vector<ContextMenu::Item> ret{
						ContextMenu::Item{
							.text = "Not Active",
							.content = [valueChangedEvent, &conditional]() {
								conditional.currentIndex = std::nullopt;
								valueChangedEvent.notify(conditional.getValue());
							},
						},
						ContextMenu::Item{
							.text{},
							.content = ContextMenu::Item::Divider{},
						},
					};

					for (auto [index, item]: std::views::enumerate(conditional.values)) {
						ret.emplace_back(ContextMenu::Item{
							.text = std::format("{}", item),
							.content = [index, valueChangedEvent, &conditional]() {
								conditional.currentIndex = index;
								valueChangedEvent.notify(conditional.getValue());
							},
						});
					}

					return ret;
				}(),
			});
		},
		.child = Row{
			.alignment = squi::Row::Alignment::center,
			.children{
				Container{
					.child = Align{
						.child = Text{
							.widget{
								.onInit = [readyEvent, valueChangedEvent, &conditional = conditional](Widget &w) {
									w.customState.add(valueChangedEvent.observe([&w, &conditional](std::optional<uint32_t> newValue) {
										auto &text = w.as<Text::Impl>();
										if (newValue.has_value()) {
											text.setText(std::format("{}: {}", conditional.prefix, newValue.value()));
											text.setColor(Color{0.f, 0.f, 0.f, 0.9f});
										} else {
											text.setText(std::format("{}: Not Active", conditional.prefix));
											text.setColor(Color{1.f, 1.f, 1.f, 0.9f});
										}
									}));
									readyEvent.notify();
								},
							},
							.text = "placeholder",
						},
					},
				},
				FontIcon{
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
				},
			},
		},
	};
}
