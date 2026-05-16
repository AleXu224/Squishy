#include "agentStats.hpp"

#include "UI/attributeToColor.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/statDisplay.hpp"
#include "UI/utils/tag.hpp"
#include "UI/utils/trueFalse.hpp"

#include "agent/data.hpp"
#include "agentEditor.hpp"
#include "agentLoadouts.hpp"
#include "store.hpp"

#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"
#include "widgets/button.hpp"
#include "widgets/image.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/stack.hpp"
#include "widgets/tooltip.hpp"


using namespace squi;
squi::core::Child UI::AgentStats::State::build(const Element &element) {
	return UI::DisplayCard{
		.widget = widget->widget,
		.borderColor = Utils::attributeToColor(widget->ctx.origin.stats.base.attribute),
		.title = std::string(widget->ctx.origin.stats.data.name),
		.children = [&]() {
			const auto &loadout = widget->ctx.source;
			Children ret{};
			ret.emplace_back(Box{
				.widget{
					.width = Size::Wrap,
					.height = Size::Wrap,
				},
				.color = Utils::attributeToColor(loadout.stats.base.attribute) * 0.5f,
				.child = Stack{
					.widget{
						.height = Size::Wrap,
					},
					.children{
						Image{
							.fit = Image::Fit::contain,
							.image = ImageProvider::fromFile(fmt::format("assets/Agents/{}/card.png", widget->ctx.origin.stats.data.name)),
						},
						Row{
							.widget{
								.width = Size::Shrink,
								.height = Size::Shrink,
								.alignment = Alignment::BottomLeft,
								.margin = 8.f,
							},
							.spacing = 4.f,
							.children{
								UI::Tag{.sourceStr = std::format("Lvl {}", loadout.stats.sheet.level)},
								UI::Tag{.sourceStr = std::format("M{}", loadout.stats.sheet.mindscape)},
							},
						},
					},
				},
			});
			std::array displayStats{Stats::agentDisplayStats, std::vector{Stats::fromAttribute(loadout.stats.base.attribute)}};

			Children ret2{};

			for (const auto &[stat, transparent]: std::views::zip(std::views::join(displayStats), Utils::trueFalse)) {
				auto formula = Stats::fromStat(Modifiers::combat(), stat);
				auto message = formula.print(widget->ctx);
				auto value = formula.eval(widget->ctx);
				ret2.emplace_back(Tooltip{
					.text = message,
					.child = UI::StatDisplay{
						.isTransparent = transparent,
						.stat{
							.stat = stat,
							.value = value,
						},
					},
				});
			}
			ret.emplace_back(Column{
				.widget{
					.padding = Padding{4.f},
				},
				.children = ret2,
			});
			return ret;
		}(),
		.footer{
			Row{
				.spacing = 4.f,
				.children{
					Button{
						.widget{
							.width = Size::Expand,
						},
						.onClick = [this]() {
							auto &agent = ::Store::agents.at(widget->agentKey);
							Navigator::of(this).pushOverlay(UI::AgentEditor{
								.agent = agent,
								.onSubmit = [](const Agent::Instance &agent) {
									auto &instance = Store::agents.at(agent.instanceKey);
									instance.state.stats.sheet = agent.state.stats.sheet;
									instance.updateEvent.notify();
								},
							});
						},
						.child = "Edit",
					},
					Button{
						.widget{
							.width = Size::Expand,
						},
						.onClick = [this]() {
							Navigator::of(this).pushOverlay(UI::AgentLoadouts{
								.agentKey = widget->agentKey,
							});
						},
						.child = "Loadouts",
					},
				},
			},
		},
	};
}
