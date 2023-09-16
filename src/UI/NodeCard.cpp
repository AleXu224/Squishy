#include "UI/NodeCard.hpp"
#include "UI/Card.hpp"
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
#include <format>
#include <variant>

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
									std::visit([&](auto &arg){
										ret = arg.name;
									}, node);
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
									reinterpret_cast<Text::Impl &>(widget).setText(std::format("{:.2f}", [&]() -> float {
										switch (storage->node.index()) {
											case 0:
												return std::get<DmgNode>(storage->node).calculate(storage->sheet);
											case 1:
												return std::get<InfoNode>(storage->node).value;
											case 2:
												return std::get<StatModifierNode>(storage->node).modifier(storage->sheet);
											default: {
												return 0.f;
											}
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

	if (auto event = observable.lock()) {
		storage->observer = event->observe([storage]() {
			storage->shouldUpdate = true;
		});
	}

	constexpr auto generateChildren = [](Nodes::NodesVec &nodes, StatSheet &sheet) -> Children {
		Children ret;

		bool isTransparent = true;
		for (auto &node: nodes) {
			bool shouldSkip;
			std::visit([&](auto &val){
				shouldSkip = !val.active;
			}, node);
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

	return Card{
		.widget = widget,
		.child = Column{
			.widget{
				.onUpdate = [talent = talent, storage](Widget &w) {
					if (talent == Talent::Passive1 && storage->character->sheet.ascension < 1) w.setVisible(false);
					else if (talent == Talent::Passive2 && storage->character->sheet.ascension < 4) w.setVisible(false);
					else w.setVisible(true);
				},
			},
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
							.onUpdate = [](Widget &w) {
								if (w.getChildren().empty()) w.setVisible(false);
							},
						},
						.children = [&]() {
							Children ret{};
							for (auto &conditional: conditionals) {
								if (conditional.second.location != talent) continue;
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
										.value = conditional.second.value,
										.onChange = [observable = observable, storage](bool value) {
											storage->character->update();
											if (auto obs = observable.lock())
												obs->notify();
										},
									},
								});
							}
							return ret;
						}(),
					},
				},
			},
		},
	};
}