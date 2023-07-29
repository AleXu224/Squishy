#include "UI/NodeCard.hpp"
#include "UI/Card.hpp"
#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "container.hpp"
#include "element.hpp"
#include "row.hpp"
#include "statSheet.hpp"
#include "text.hpp"
#include <format>
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
				.height = 36.f,
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
						Text{
							.text = [&]() -> std::string {
								switch (node.index()) {
									case 0: {
										return std::get<DmgNode>(node).name;
									}
									case 1: {
										return std::get<InfoNode>(node).name;
									}
									default: {
										return "Unknown";
									}
								}
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
	return Card{
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
					},
					.spacing = 4.f,
					.children = [&]() -> Children {
						std::vector<Child> ret;

						bool isTransparent = true;
						for (auto &node: nodes) {
							ret.emplace_back(NodeDisplay{
								.node = node,
								.sheet = sheet,
								.isTransparent = isTransparent,
							});
							isTransparent = !isTransparent;
						}

						return ret;
					}(),
				},
			},
		},
	};
}