#include "nodePicker.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/masonry.hpp"
#include "align.hpp"
#include "button.hpp"
#include "character/data.hpp"
#include "container.hpp"
#include "dialog.hpp"
#include "node/node.hpp"
#include "ranges"
#include "stats/loadout.hpp"
#include "store.hpp"
#include "text.hpp"
#include "utils/slotToCondition.hpp"
#include "weapon/data.hpp"


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

		operator squi::Child() const {
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
						.color = Node::getColor(node.data, ctx),
					},
				},
			};
		}
	};

	struct NodePickerGroup {
		// Args
		squi::Widget::Args widget{};
		Children children;

		operator squi::Child() const {
			return UI::DisplayCard{
				.title = "Category",
				.children = children,
			};
		}
	};
}// namespace

UI::NodePicker::operator squi::Child() const {
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

				Children transformativeRet;
				for (const auto &reaction: Misc::transformativeReactions) {
					auto source = Combo::Source::TransformativeReaction{reaction};
					auto node = source.resolve({});
					transformativeRet.emplace_back(NodePickerEntry{
						.node = node,
						.source = source,
						.ctx = ctx,
						.onSelect = onSelect,
						.closeEvent = closeEvent,
					});
				}
				if (!transformativeRet.empty()) {
					ret.emplace_back(DisplayCard{
						.title = "Transformative reactions",
						.children = transformativeRet,
					});
				}

				if (enableCombos) {
					Children comboRet{};
					for (const auto &[key, combo]: character.combos) {
						auto source = Combo::Source::Combo{
							.characterKey = character.instanceKey,
							.comboKey = key,
						};
						auto node = source.resolve({});
						comboRet.emplace_back(NodePickerEntry{
							.node = node,
							.source = source,
							.ctx = ctx,
							.onSelect = onSelect,
							.closeEvent = closeEvent,
						});
					}
					if (!comboRet.empty()) {
						ret.emplace_back(DisplayCard{
							.title = "Combos",
							.children = comboRet,
						});
					}
				}

				for (const auto &slot: Node::characterSlots) {
					if (!Utils::slotToCondition(slot).eval(ctx)) continue;
					Children entryRet{};
					const auto &nodeList = character.state.stats.data.data.nodes.fromEntry(slot);
					for (const auto &[index, node]: nodeList | std::views::enumerate) {
						if (!Node::getOptimizable(node.data)) continue;
						entryRet.emplace_back(NodePickerEntry{
							.node = node,
							.source = Combo::Source::Character{
								.key = character.dataKey,
								.slot = slot,
								.index = static_cast<size_t>(index),
							},
							.ctx = ctx,
							.onSelect = onSelect,
							.closeEvent = closeEvent,
						});
					}

					if (!entryRet.empty()) {
						ret.emplace_back(DisplayCard{
							.title = Utils::Stringify(slot),
							.children = entryRet,
						});
					}
				}

				Children weaponRet{};
				for (const auto &[index, node]: character.state.loadout().weapon->data->data.nodes | std::views::enumerate) {
					if (!Node::getOptimizable(node.data)) continue;
					weaponRet.emplace_back(NodePickerEntry{
						.node = node,
						.source = Combo::Source::Weapon{
							.key = character.state.loadout().weapon->data->key,
							.index = static_cast<size_t>(index),
						},
						.ctx = ctx,
						.onSelect = onSelect,
						.closeEvent = closeEvent,
					});
				}
				if (!weaponRet.empty()) {
					ret.emplace_back(DisplayCard{
						.title = character.state.loadout().weapon->data->name,
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
