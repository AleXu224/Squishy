#include "agentAnomalies.hpp"

#include "UI/attributeToColor.hpp"
#include "UI/utils/descriptorDisplay.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/skillEntry.hpp"
#include "agent/data.hpp"
#include "anomaly/anomaly.hpp"
#include "anomaly/list.hpp"


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
				if (anomaly->attribute.eval(ctx) != state.stats.data.baseStats.attribute) continue;
				ret.emplace_back(DescriptorDisplay{
					.descriptorProvider = [this, anomaly]() {
						return anomaly->formula.print(ctx, Formula::Step::none);
					},
					.child = UI::SkillEntry{
						.isTransparent = (transparent = !transparent),
						.name = anomaly->name.eval(ctx),
						.value = anomaly->formula.eval(ctx),
						.color = Utils::attributeToColor(anomaly->attribute.eval(ctx)),
					},
				});
			}
			for (const auto &anomaly: Anomaly::List::disorderList) {
				if (anomaly->attribute.eval(ctx) != state.stats.data.baseStats.attribute) continue;
				ret.emplace_back(DescriptorDisplay{
					.descriptorProvider = [this, anomaly]() {
						return anomaly->formula.print(ctx, Formula::Step::none);
					},
					.child = UI::SkillEntry{
						.isTransparent = (transparent = !transparent),
						.name = anomaly->name.eval(ctx),
						.value = anomaly->formula.eval(ctx),
						.color = Utils::attributeToColor(anomaly->attribute.eval(ctx)),
					},
				});
			}

			if (ctx.origin.stats.base.attribute == Misc::Attribute::wind) {
				for (const auto &anomaly: Anomaly::List::vortexList()) {
					auto val = anomaly.formula.eval(ctx);
					if (val <= 0.f) continue;

					ret.emplace_back(DescriptorDisplay{
						.descriptorProvider = [this, &anomaly]() {
							return anomaly.formula.print(ctx, Formula::Step::none);
						},
						.child = UI::SkillEntry{
							.isTransparent = (transparent = !transparent),
							.name = anomaly.name.eval(ctx),
							.value = val,
							.color = Utils::attributeToColor(anomaly.attribute.eval(ctx)),
						},
					});
				}
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
