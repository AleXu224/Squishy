#include "valueListOption.hpp"

#include "align.hpp"
#include "button.hpp"
#include "column.hpp"
#include "container.hpp"
#include "contextMenu.hpp"
#include "fontIcon.hpp"
#include "row.hpp"
#include "store.hpp"
#include "text.hpp"

#include "Ui/utils/decodeModsSheet.hpp"

#include "ranges"
#include "vector"

using namespace squi;

UI::ValueListOption::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();
	squi::Observable<std::optional<uint32_t>, std::optional<uint32_t>> internalValueChangedEvent{};
	CountObserver readyEvent(2);

	auto buttonText = Text{
		.widget{
			.onInit = [readyEvent, internalValueChangedEvent, &option = option](Widget &w) {
				w.customState.add(internalValueChangedEvent.observe([&w, &option](std::optional<uint32_t> newValue, std::optional<uint32_t> index) {
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
			.onInit = [readyEvent, internalValueChangedEvent](Widget &w) {
				w.customState.add(internalValueChangedEvent.observe([&w](std::optional<uint32_t> newValue, std::optional<uint32_t> index) {
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
		.icon = 0xe313,
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

	auto mods = decodeOption(option, ctx);
	auto borderRadiusFunc = [hasMods = !mods.empty()](ButtonStyle style) {
		if (hasMods) style.borderRadius = style.borderRadius.withBottom(0.f);
		return style;
	};

	return Column{
		.widget = widget,
		.children{
			Button{
				.widget{
					.width = Size::Expand,
					.height = Size::Shrink,
					.sizeConstraints{
						.minHeight = 32.f,
					},
					.onInit = [readyEvent, internalValueChangedEvent, valueChangedEvent = valueChangedEvent, &option = option, borderRadiusFunc](Widget &w) {
						observe("valueChangedEvent", w, valueChangedEvent, [internalValueChangedEvent](std::optional<uint32_t> value, std::optional<uint32_t> index) {
							internalValueChangedEvent.notify(value, index);
						});
						w.customState.add(internalValueChangedEvent.observe([&w, borderRadiusFunc](std::optional<uint32_t> newVal, std::optional<uint32_t> index) {
							auto style = ButtonStyle::Standard();

							if (newVal.has_value()) style = ButtonStyle::Accent();
							Button::State::style.of(w) = borderRadiusFunc(style);
						}));
						w.customState.add(readyEvent.observe([internalValueChangedEvent, &option]() {
							internalValueChangedEvent.notify(option.getValue(), option.currentIndex);
						}));
					},
				},
				.style = option.getValue().has_value() ? borderRadiusFunc(ButtonStyle::Accent()) : borderRadiusFunc(ButtonStyle::Standard()),
				.onClick = [valueChangedEvent = valueChangedEvent, &option = option, instanceKey = instanceKey](GestureDetector::Event event) {
					event.widget.addOverlay(ContextMenu{
						.position = event.widget.getPos().withYOffset(event.widget.getSize().y),
						.items = [&]() {
							std::vector<ContextMenu::Item> ret{
								ContextMenu::Item{
									.text = "Not Active",
									.content = [valueChangedEvent, &option, instanceKey]() {
										option.currentIndex = std::nullopt;
										valueChangedEvent.notify(option.getValue(), option.currentIndex);
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
								},
								ContextMenu::Item{
									.text{},
									.content = ContextMenu::Item::Divider{},
								},
							};

							for (const auto &[index, item]: std::views::enumerate(option.values)) {
								ret.emplace_back(ContextMenu::Item{
									.text = std::format("{}", item),
									.content = [index, valueChangedEvent, &option, instanceKey]() {
										option.currentIndex = index;
										valueChangedEvent.notify(option.getValue(), option.currentIndex);
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
								});
							}

							return ret;
						}(),
					});
				},
				.child = buttonContent,
			},
			Box{
				.widget{
					.onInit = [mods](Widget &w) {
						w.flags.visible = !mods.empty();
					},
				},
				.color = Color::css(0x0, 0.3f),
				.borderRadius = BorderRadius::Bottom(4.f),
				.child = Column{
					.widget{
						.padding = 4.f,
					},
					.children = mods,
				},
			},
		},
	};
}
