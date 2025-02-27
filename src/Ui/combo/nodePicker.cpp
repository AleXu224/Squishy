#include "nodePicker.hpp"
#include "Ui/elementToColor.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/masonry.hpp"
#include "align.hpp"
#include "button.hpp"
#include "container.hpp"
#include "dialog.hpp"
#include "formula/element.hpp"
#include "node/node.hpp"
#include "ranges"
#include "stats/loadout.hpp"
#include "store.hpp"
#include "text.hpp"


using namespace squi;

namespace {
	struct NodePickerEntry {
		// Args
		squi::Widget::Args widget{};
		const Node::Instance &node;
		Combo::Source::Types source;
		const Formula::Context &ctx;
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
				.style = ButtonStyle::Subtle(),
				.onClick = [onSelect = onSelect, source = source, closeEvent = closeEvent](auto) {
					onSelect(source);
					closeEvent.notify();
				},
				.child = Align{
					.xAlign = 0.f,
					.child = Text{
						.text = node.name,
						.lineWrap = true,
						.color = [&]() {
							Color ret;

							std::visit(
								Utils::overloaded{
									[&](const Node::AtkData &data) {
										auto element = Formula::getElement(data.source, data.element, ctx);
										ret = Utils::elementToColor(element);
									},
									[&](const Node::HealData &data) {
										ret = Utils::elementToColor(Misc::Element::dendro);
									},
									[&](const Node::InfoData &data) {
										ret = data.color;
									},
								},
								node.data
							);

							return ret;
						}(),
					},
				},
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
		.width = 800.f,
		.closeEvent = closeEvent,
		.title = "Choose node",
		.content = Masonry{
			.spacing = 4.f,
			.columnCount = Masonry::MinSize{200.f},
			.children = [&]() {
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
							.ctx = ctx,
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

				Children weaponRet{};
				for (const auto &[index, node]: character.loadout.weapon.data->data.nodes | std::views::enumerate) {
					weaponRet.emplace_back(NodePickerEntry{
						.node = node,
						.source = Combo::Source::Weapon{
							.key = character.loadout.weapon.data->key,
							.index = static_cast<size_t>(index),
						},
						.ctx = ctx,
						.onSelect = onSelect,
						.closeEvent = closeEvent,
					});
				}
				if (!weaponRet.empty()) {
					ret.emplace_back(DisplayCard{
						.title = character.loadout.weapon.data->name,
						.children = weaponRet,
					});
				}

				// FIXME: add artifacts

				return ret;
			}(),
		},
		.buttons{
			Container{},
			Button{
				.widget{.width = Size::Expand},
				.text = "Close",
				.onClick = [closeEvent](auto) {
					closeEvent.notify();
				},
			},
		},
	};
}
