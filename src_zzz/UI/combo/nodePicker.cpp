#include "nodePicker.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/masonry.hpp"
#include "agent/data.hpp"
#include "engine/data.hpp"
#include "node/node.hpp"
#include "ranges"
#include "stats/loadout.hpp"
#include "store.hpp"
#include "utils/slotToCondition.hpp"
#include "widgets/button.hpp"
#include "widgets/dialog.hpp"
#include "widgets/text.hpp"


using namespace squi;

namespace {
	struct NodePickerEntry : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Node::Instance node;
		Combo::Source::Types source;
		Formula::Context ctx;
		std::function<void(Combo::Source::Types)> onSelect;
		VoidObservable closeEvent;

		[[nodiscard]] Child build(const Element &) const {
			return Button{
				.widget{
					.width = Size::Expand,
					.margin = Margin{4.f, 2.f},
				},
				.theme = Button::Theme::Subtle(),
				.onClick = [this]() {
					if (onSelect) onSelect(source);
					closeEvent.notify();
				},
				.child = Text{
					.widget{
						.alignment = Alignment::CenterLeft,
					},
					.text = std::string(node.name),
					.lineWrap = true,
					.color = Node::getColor(node.data, ctx),
				},
			};
		}
	};
}// namespace

squi::core::Child UI::NodePicker::State::build(const Element &element) {
	return Dialog{
		.width = 800.f,
		.closeEvent = closeEvent,
		.title = "Choose node",
		.content = Masonry{
			.columnCount = Masonry::MinSize{200.f},
			.spacing = 4.f,
			.children = [&]() {
				Children ret;

				auto &agent = ::Store::agents.at(widget->agentKey);

				Children anomalyRet;
				for (const auto &anomaly: Misc::anomalies) {
					auto source = Combo::Source::Anomaly{anomaly};
					auto node = source.resolve({});
					anomalyRet.emplace_back(NodePickerEntry{
						.node = node,
						.source = source,
						.ctx = widget->ctx,
						.onSelect = widget->onSelect,
						.closeEvent = closeEvent,
					});
				}
				if (!anomalyRet.empty()) {
					ret.emplace_back(DisplayCard{
						.title = "Anomalies",
						.children = anomalyRet,
					});
				}

				if (widget->enableCombos) {
					Children comboRet{};
					for (const auto &[key, combo]: agent.combos) {
						auto source = Combo::Source::Combo{
							.agentKey = agent.instanceKey,
							.comboKey = key,
						};
						auto node = source.resolve({});
						comboRet.emplace_back(NodePickerEntry{
							.node = node,
							.source = source,
							.ctx = widget->ctx,
							.onSelect = widget->onSelect,
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

				for (const auto &slot: Node::agentSlots) {
					if (!Utils::slotToCondition(slot).eval(widget->ctx)) continue;
					Children entryRet{};
					const auto &nodeList = agent.state.stats.data.data->nodes.fromEntry(slot);
					for (const auto &[index, node]: nodeList | std::views::enumerate) {
						if (!Node::getOptimizable(node.data)) continue;
						entryRet.emplace_back(NodePickerEntry{
							.node = node,
							.source = Combo::Source::Agent{
								.key = agent.dataKey,
								.slot = slot,
								.index = static_cast<size_t>(index),
							},
							.ctx = widget->ctx,
							.onSelect = widget->onSelect,
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

				Children engineRet{};
				if (agent.state.loadout().engine) {
					for (const auto &[index, node]: agent.state.loadout().engine->data->data.nodes | std::views::enumerate) {
						if (!Node::getOptimizable(node.data)) continue;
						engineRet.emplace_back(NodePickerEntry{
							.node = node,
							.source = Combo::Source::Engine{
								.key = agent.state.loadout().engine->data->key,
								.index = static_cast<size_t>(index),
							},
							.ctx = widget->ctx,
							.onSelect = widget->onSelect,
							.closeEvent = closeEvent,
						});
					}
				}
				if (!engineRet.empty()) {
					ret.emplace_back(DisplayCard{
						.title = agent.state.loadout().engine->data->name,
						.children = engineRet,
					});
				}

				// FIXME: add discs

				return ret;
			}(),
		},
		.buttons{
			Button{
				.widget{.width = Size::Expand},
				.onClick = [this]() {
					closeEvent.notify();
				},
				.child = "Close",
			},
		},
	};
}
