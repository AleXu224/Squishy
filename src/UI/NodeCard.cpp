#include "UI/NodeCard.hpp"
#include "UI/Card.hpp"
#include "UI/DropdownButton.hpp"
#include "align.hpp"
#include "box.hpp"
#include "button.hpp"
#include "checkbox.hpp"
#include "column.hpp"
#include "container.hpp"
#include "element.hpp"
#include "node.hpp"
#include "row.hpp"
#include "statSheet.hpp"
#include "text.hpp"
#include "widget.hpp"
#include "wrapper.hpp"
#include <format>
#include <functional>
#include <variant>
#include <vector>


using namespace squi;
using namespace Squishy;

struct NodeDisplay {
	// Args
	Node &node;
	StatSheet &sheet;
	bool isTransparent;

	struct Storage {
		// Data
		Node &node;
		StatSheet &sheet;
	};

	operator Child() const {
		auto storage = std::make_shared<Storage>(node, sheet);

		return Box{
			.widget{
				.height = Size::Shrink,
				.sizeConstraints{
					.minHeight = 36.f,
				},
			},
			.color = isTransparent ? Color::HEX(0x00000000) : Color::RGBA(1, 1, 1, 0.04),
			.borderRadius = 4.f,
			.child{
				Row{
					.widget{
						.padding = Padding{0.f, 12.f, 0.f, 12.f},
					},
					.alignment = Row::Alignment::center,
					.children{
						Container{
							.widget{
								.height = Size::Shrink,
							},
							.child = Text{
								.text = [&]() -> std::string {
									std::string ret;
									std::visit([&](auto &arg) {
										ret = arg.name;
									},
											   node);
									return ret;
								}(),
								.fontSize = 14.f,
								.lineWrap = true,
								.color = [&]() -> Color {
									switch (node.index()) {
										case 0: {
											return elementColors.at(std::get<DmgNode>(node).getDMGElement(sheet));
										}
										default: {
											return Color::HEX(0xFFFFFFFF);
										}
									}
								}(),
							},
						},
						Container{
							.widget{
								.width = 12.f,
							},
						},
						Text{
							.widget{
								.onUpdate = [storage](Widget &widget) {
									reinterpret_cast<Text::Impl &>(widget).setText(std::format(
										"{:.2f}{}", [&]() -> float {
											switch (storage->node.index()) {
												case 0:
													return std::get<DmgNode>(storage->node).calculate(storage->sheet);
												case 1:
													return std::get<InfoNode>(storage->node).value * (std::get<InfoNode>(storage->node).isPercentage ? 100.f : 1.f);
												case 2:
													return std::get<StatModifierNode>(storage->node).modifier(storage->sheet);
												default: {
													return 0.f;
												}
											}
										}(),
										[&]() -> std::string {
											switch (storage->node.index()) {
												case 1:
													return std::get<InfoNode>(storage->node).isPercentage ? "%" : "";
												default:
													return "";
											}
										}()));
								},
							},
							.fontSize = 14.f,
						},
					},
				},
			},
		};
	}
};

NodeCard::operator Child() const {
	auto storage = std::make_shared<Storage>(nodes, character);

	if (auto event = statsChangedEvent.lock()) {
		storage->observer = event->observe([storage]() {
			storage->shouldUpdate = true;
		});
	}

	constexpr auto generateChildren = [](Nodes::NodesVec &nodes, StatSheet &sheet) -> Children {
		Children ret;

		bool isTransparent = true;
		for (auto &node: nodes) {
			bool shouldSkip;
			std::visit([&](auto &val) {
				shouldSkip = !val.active;
			},
					   node);
			if (shouldSkip) continue;
			ret.emplace_back(NodeDisplay{
				.node = node,
				.sheet = sheet,
				.isTransparent = isTransparent,
			});
			isTransparent = !isTransparent;
		}

		return ret;
	};

	return Wrapper{
		.onUpdate = [talent = talent, storage](Widget &w) {
			if (!storage->hasConditionals && !storage->hasNodes)
				w.setVisible(false);
			else if (talent == Talent::Passive1 && storage->character->sheet.ascension < 1)
				w.setVisible(false);
			else if (talent == Talent::Passive2 && storage->character->sheet.ascension < 4)
				w.setVisible(false);
			else if (talent == Talent::Constellation1 && storage->character->sheet.constellation < 1)
				w.setVisible(false);
			else if (talent == Talent::Constellation2 && storage->character->sheet.constellation < 2)
				w.setVisible(false);
			else if (talent == Talent::Constellation3 && storage->character->sheet.constellation < 3)
				w.setVisible(false);
			else if (talent == Talent::Constellation4 && storage->character->sheet.constellation < 4)
				w.setVisible(false);
			else if (talent == Talent::Constellation5 && storage->character->sheet.constellation < 5)
				w.setVisible(false);
			else if (talent == Talent::Constellation6 && storage->character->sheet.constellation < 6)
				w.setVisible(false);
			else
				w.setVisible(true);
		},
		.child = Card{
			.widget = widget,
			.child = Column{
				.children{
					Box{
						.widget{
							.height = 64.f,
							.padding = Padding{0.f, 16.f, 0.f, 16.f},
						},
						.color = Color::RGBA(1, 1, 1, 0.08),
						.shouldClipContent = false,
						.child = Align{
							.xAlign = 0.f,
							.yAlign = 0.5f,
							.child = Text{
								.text = std::string{name},
								.fontSize = 24.f,
							},
						},
					},
					Column{
						.widget{
							.height = Size::Shrink,
							.margin = 4.f,
							.onUpdate = [generateChildren, storage](Widget &widget) {
								if (storage->shouldUpdate) {
									widget.setChildren(generateChildren(storage->nodes, storage->character->sheet));
									storage->shouldUpdate = false;
								}
								if (widget.getChildren().empty()) {
									widget.setVisible(false);
									storage->hasNodes = false;
								} else {
									widget.setVisible(true);
									storage->hasNodes = true;
								}
							},
						},
						.spacing = 4.f,
						.children = generateChildren(nodes, character->sheet),
					},
					Box{
						.widget{
							.height = Size::Shrink,
						},
						.color = Color::RGBA(1, 1, 1, 0.08),
						.child = Column{
							.widget{
								.margin = 8.f,
								.onInit = [storage](Widget &w) {
									if (w.getChildren().empty()) {
										w.setVisible(false);
										storage->hasConditionals = false;
									}
								},
							},
							.children = [&]() {
								Children ret{};
								for (auto &conditional: conditionals) {
									if (conditional.second.location != talent) continue;
									if (conditional.second.values.empty()) {
										ret.emplace_back(Button{
											.widget{
												.width = Size::Expand,
												.height = Size::Shrink,
												.padding = 0.f,
											},
											.style = ButtonStyle::Subtle(),
											.child = Checkbox{
												.widget{
													.width = Size::Expand,
													.padding = Padding{10.f, 2.f},
												},
												.text = conditional.second.name,
												.value = conditional.second.active,
												.onChange = [observable = statsChangedEvent, storage](bool value) {
													storage->character->update();
													if (auto obs = observable.lock())
														obs->notify();
												},
											},
										});
									} else {
										ret.emplace_back(Row{
											.widget{
												.height = Size::Shrink,
											},
											.alignment = Row::Alignment::center,
											.children{
												Text{
													.text = conditional.second.name,
													.lineWrap = true,
												},
												DropdownButton<float>{
													.value = conditional.second.value,
													.onSelect = [observable = statsChangedEvent, storage, &conditional = conditional.second](const DropdownButton<float>::Item &item) {
														conditional.active = item.value != 0.f;
														conditional.value = item.value;
														storage->character->update();
														if (auto obs = observable.lock())
															obs->notify();
													},
													.items = std::invoke([&]() -> std::vector<DropdownButton<float>::Item> {
														std::vector<DropdownButton<float>::Item> ret{};
														ret.reserve(conditional.second.values.size() + 1);
														ret.emplace_back("Inactive", 0.f);
														for (auto &value: conditional.second.values) {
															ret.emplace_back(std::format("{:.2}", value), value);
														}
														return ret;
													}),
												},
											},
										});
									}
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