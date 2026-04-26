#include "agentCard.hpp"

#include "UI/utils/card.hpp"
#include "UI/utils/statDisplay.hpp"
#include "UI/utils/trueFalse.hpp"
#include "agentCardBanner.hpp"
#include "agentEditor.hpp"
#include "agentPage.hpp"
#include "formula/stat.hpp"
#include "store.hpp"

#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/gestureDetector.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"

using namespace squi;

struct AgentCardsContents : StatelessWidget {
	// Args
	Key key;
	Agent::InstanceKey agentKey;

	[[nodiscard]] Child build(const Element &element) const {
		auto &agent = ::Store::agents.at(agentKey);
		auto statsToDisplay = std::vector{Stats::agentDisplayStats, {Stats::fromAttribute(agent.state.stats.base.attribute)}};
		Team::Instance placeholderTeam{};
		placeholderTeam.stats.agents.at(0) = &agent;
		Formula::Context ctx{
			.source = agent.state,
			.active = agent.state,
			.team = placeholderTeam.stats,
			.enemy = ::Store::enemies.at(0).stats,
		};

		return Column{
			.children{
				Gesture{
					.onClick = [&](Gesture::State) {
						Navigator::of(element).push(UI::AgentPage{
							.agentKey = agentKey,
						});
					},
					.child = UI::AgentCardBanner{
						.agentKey = agentKey,
					},
				},
				Column{
					.children = [&]() {
						Children ret;
						for (const auto &[stat, transparent]: std::views::zip(
								 std::views::join(statsToDisplay),
								 Utils::trueFalse
							 )) {
							auto formula = Formula::Stat{.stat = stat};
							ret.emplace_back(UI::StatDisplay{
								.isTransparent = transparent,
								.stat = StatValue{.stat = stat, .value = formula.eval(ctx)},
							});
						}
						return ret;
					}(),
				},
				Row{
					.widget{
						.height = Size::Shrink,
						.padding = 4.f,
					},
					.spacing = 4.f,
					.children{
						Button{
							.theme = Button::Theme::Standard(),
							.onClick = [this, &element]() {
								auto &agent = ::Store::agents.at(agentKey);
								Navigator::of(element).pushOverlay(UI::AgentEditor{
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
							.theme = Button::Theme::Standard(),
							.onClick = [this]() {
								auto &agent = Store::agents.at(agentKey);
								std::get<Stats::Disc::Slotted>(agent.state.equippedLoadout.disc.equipped).unequipAll();
								Store::agents.erase(agentKey);
								Store::agentListUpdateEvent.notify();
							},
							.child = "Delete",
						},
					},
				}
			},
		};
	}
};

squi::core::Child UI::AgentCard::State::build(const Element &element) {
	return Card{
		.child = AgentCardsContents{
			.agentKey = widget->agentKey,
		},
	};
}
