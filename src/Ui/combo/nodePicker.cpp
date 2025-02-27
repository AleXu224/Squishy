#include "nodePicker.hpp"
#include "Ui/utils/displayCard.hpp"
#include "button.hpp"
#include "column.hpp"
#include "dialog.hpp"
#include "node/node.hpp"
#include "ranges"
#include "stats/loadout.hpp"
#include "store.hpp"


using namespace squi;

namespace {
	struct NodePickerEntry {
		// Args
		squi::Widget::Args widget{};
		const Node::Instance &node;
		Combo::Source::Types source;
		std::function<void(Combo::Source::Types)> onSelect;
		VoidObservable closeEvent;

		struct Storage {
			// Data
		};

		operator squi::Child() const {
			auto storage = std::make_shared<Storage>();

			return Button{
				.widget{
					.width = Size::Expand,
					.margin = Margin{4.f, 2.f},
				},
				.text = node.name,
				.style = ButtonStyle::Subtle(),
				.onClick = [onSelect = onSelect, source = source, closeEvent = closeEvent](auto) {
					onSelect(source);
					closeEvent.notify();
				}
			};
		}
	};

	struct NodePickerGroup {
		// Args
		squi::Widget::Args widget{};
		Children children;

		struct Storage {
			// Data
		};

		operator squi::Child() const {
			auto storage = std::make_shared<Storage>();

			return UI::DisplayCard{
				.title = "Category",
				.children = children,
			};
		}
	};
}// namespace

UI::NodePicker::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	VoidObservable closeEvent;

	return Dialog{
		.closeEvent = closeEvent,
		.title = "Choose node",
		.content = Column{
			.children = [characterKey = characterKey, onSelect = onSelect, closeEvent = closeEvent]() {
				Children ret;

				auto &character = ::Store::characters.at(characterKey);

				for (const auto &entry: Node::characterEntries) {
					Children entryRet{};
					const auto &nodeList = character.loadout.character.data.data.nodes.fromEntry(entry);
					for (const auto &[index, node]: nodeList | std::views::enumerate) {
						entryRet.emplace_back(NodePickerEntry{
							.node = node,
							.source = Combo::Source::Character{
								.key = character.dataKey,
								.slot = entry,
								.index = static_cast<size_t>(index),
							},
							.onSelect = onSelect,
							.closeEvent = closeEvent,
						});
					}

					if (!entryRet.empty()) {
						ret.emplace_back(DisplayCard{
							.title = Utils::Stringify(entry),
							.children = entryRet,
						});
					}
				}

				return ret;
			}(),
		},
		.buttons{
			Button{
				.text = "Close",
				.onClick = [closeEvent](auto) {
					closeEvent.notify();
				},
			},
		},
	};
}
