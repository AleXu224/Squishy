#pragma once
#include "button.hpp"
#include "contextMenu.hpp"
#include "fontIcon.hpp"
#include "row.hpp"
#include "widget.hpp"
#include "window.hpp"
#include <format>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>


namespace Squishy {
	template<typename T>
	struct DropdownButton {
		// Args
		squi::Widget::Args widget{};
		squi::ButtonStyle style = squi::ButtonStyle::Standard();
		std::string_view text{""};
		struct Item {
			std::string name{};
			T value{};
		};
        // Automatically update this value when an item is selected
        std::optional<std::reference_wrapper<T>> value{};
		std::function<void(const Item &)> onSelect{};
		std::vector<Item> items{};

		struct Storage {
			// Data
			bool disabled = false;
            std::optional<Item> selected{};
            std::optional<std::reference_wrapper<T>> value{};
            std::function<void(const Item &)> onSelect{};
            std::vector<Item> items{};
		};


		operator squi::Child() const {
			using namespace squi;
			auto storage = std::make_shared<Storage>(Storage{.value = value, .onSelect = onSelect, .items = items});

			if (storage->value) {
                for (auto &item : items) {
                    if (item.value == *storage->value) {
                        storage->selected = item;
                        break;
                    }
                }
            }
			return Button{
				.widget{widget},
				.style = style,
				.onClick = [storage](GestureDetector::Event event) {
					auto window = dynamic_cast<Window *>(event.widget.state.root);
					window->addOverlay(ContextMenu{
						.position = [&]() -> vec2 {
							const auto rect = event.widget.getRect();
                            return vec2{rect.left, rect.bottom};
						}(),
						.items = [storage]() -> std::vector<ContextMenu::Item> {
                            std::vector<ContextMenu::Item> ret{};
                            for (auto &item : storage->items) {
                                ret.push_back(ContextMenu::Item{
                                    .text = item.name,
                                    .content = [storage, item = item]() {
                                        if (storage->value) storage->value.value().get() = item.value;
                                        if (storage->onSelect) storage->onSelect(item);
                                        storage->selected = item;
                                    },
                                });
                            }
                            return ret;
						}(),
					});
				},
				.child = Row{
					.widget{
						.width = Size::Shrink,
					},
					.alignment = Row::Alignment::center,
					.spacing = 4.f,
					.children{
						Text{
                            .widget{
                                .onUpdate = [name = text, storage](Widget &w){
                                    if (storage->selected && storage->value) {
                                        T &value = storage->value.value().get();
                                        T &selected = storage->selected->value;
                                        if (value != selected) {
                                            for (auto &item : storage->items) {
                                                if (item.value == value) {
                                                    storage->selected = item;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    
                                    auto &textWidget = dynamic_cast<Text::Impl &>(w);

                                    if (storage->selected) {
										if (!name.empty())
											textWidget.setText(std::format("{}: {}", name, storage->selected->name));
										else
											textWidget.setText(storage->selected->name);
                                    } else {
                                        textWidget.setText(name);
                                    }
                                },
                            },
							.text = text,
						},
						FontIcon{
							.icon = 0xE972,
							.size = 12.f,
						},
					},
				},
			};
		}
	};
}// namespace Squishy