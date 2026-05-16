#include "agentAnomalies.hpp"

#include "UI/attributeToColor.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/skillEntry.hpp"
#include "agent/data.hpp"
#include "anomaly/anomaly.hpp"
#include "anomaly/list.hpp"
#include "widgets/gestureDetector.hpp"
#include "widgets/tooltip.hpp"


using namespace squi;

[[nodiscard]] squi::core::Child UI::AgentAnomalies::build(const Element &) const {
	return UI::DisplayCard{
		.widget = widget,
		.title = "Anomalies",
		.children = [&]() -> Children {
			Children ret;
			const auto &state = ctx.origin;
			// auto reactions = Reaction::List::Transformative::getMembers();

			bool transparent = true;
			for (const auto &anomaly: Anomaly::List::anomalyList) {
				if (anomaly->attribute != state.stats.data.baseStats.attribute) continue;
				ret.emplace_back(Gesture{
					.onClick = [out = anomaly->formula.fold(ctx, {}).print(ctx)](const Gesture::State &state) {
						std::println("{}", out);
					},
					.child = Tooltip{
						.text = anomaly->formula.print(ctx, Formula::Step::none),
						.child = UI::SkillEntry{
							.isTransparent = (transparent = !transparent),
							.name = std::string(anomaly->name),
							.value = anomaly->formula.eval(ctx),
							.color = Utils::attributeToColor(anomaly->attribute),
						},
					},
				});
			}
			for (const auto &anomaly: Anomaly::List::disorderList) {
				if (anomaly->attribute != state.stats.data.baseStats.attribute) continue;
				ret.emplace_back(Tooltip{
					.text = anomaly->formula.print(ctx, Formula::Step::none),
					.child = UI::SkillEntry{
						.isTransparent = (transparent = !transparent),
						.name = std::string(anomaly->name),
						.value = anomaly->formula.eval(ctx),
						.color = Utils::attributeToColor(anomaly->attribute),
					},
				});
			}

			return Children{
				Column{
					.widget{
						.padding = Padding{4.f},
					},
					.children = ret,
				},
			};
		}(),
	};
}
