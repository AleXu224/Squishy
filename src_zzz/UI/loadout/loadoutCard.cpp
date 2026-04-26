#include "loadoutCard.hpp"
#include "UI/disc/discCard.hpp"
#include "UI/engine/engineCard.hpp"
#include "UI/utils/card.hpp"
#include "store.hpp"
#include "widgets/button.hpp"
#include "widgets/expander.hpp"
#include "widgets/row.hpp"
#include "widgets/tooltip.hpp"


using namespace squi;

squi::core::Child UI::LoadoutCard::State::build(const Element &element) {
	auto &agent = Store::agents.at(widget->agentKey);
	auto &loadout = agent.state.loadoutByIndex(widget->loadoutIndex);

	return Expander{
		.title = widget->loadoutIndex.has_value() ? "Loadout name" : "Equipped",
		.alwaysExpanded = true,
		.action = Row{
			.widget{
				.width = Size::Wrap,
			},
			.crossAxisAlignment = Row::Alignment::center,
			.spacing = 4.f,
			.children{
				widget->loadoutIndex.has_value() && std::holds_alternative<Stats::Disc::Slotted>(loadout.disc.equipped)//
					? Tooltip{
						  .text = "Sets the equipped build to this build. If any of the discs in this build are equipped on other agents, they will be swapped with this agent's.",
						  .child = Button{
							  .onClick = [this, &agent]() {
								  auto &loadout = agent.state.loadoutByIndex(widget->loadoutIndex);
								  setState([&]() {
									  agent.state.equippedLoadout.swapEngine(loadout.engineInstanceKey);
									  if (!std::holds_alternative<::Stats::Disc::Slotted>(loadout.disc.equipped)) return;
									  auto &slotted = std::get<::Stats::Disc::Slotted>(loadout.disc.equipped);
									  for (const auto &partition: Disc::partitions) {
										  auto &art = slotted.fromPartition(partition);
										  if (art) Store::discs.at(art).equipOn(agent.instanceKey);
									  }
								  });
							  },
							  .child = "Equip",
						  },
					  }
					: Child{},
				Button{
					.disabled = agent.state.loadoutIndex == widget->loadoutIndex,
					.onClick = [this, &agent]() {
						agent.state.loadoutIndex = widget->loadoutIndex;
						agent.updateEvent.notify();
						agent.loadoutChangedEvent.notify();
					},
					.child = "Activate",
				},
				widget->loadoutIndex.has_value()//
					? Button{
						  .theme = Button::Theme::Standard(),
						  .disabled = agent.state.loadoutIndex == widget->loadoutIndex,
						  .onClick = [this, &agent]() {
							  setState([&]() {
								  if (agent.state.loadoutIndex.has_value() && agent.state.loadoutIndex.value() > widget->loadoutIndex.value()) {
									  agent.state.loadoutIndex.value()--;
								  }
								  agent.state.loadouts.erase(agent.state.loadouts.begin() + widget->loadoutIndex.value());
							  });
							  agent.updateEvent.notify();
							  agent.loadoutChangedEvent.notify();
						  },
						  .child = "Remove",
					  }
					: Child{},
			},
		},
		.content = Row{
			.widget{
				.width = Size::Expand,
				.padding = 8.f,
			},
			.spacing = 8.f,
			.children = [&]() {
				Children ret{};
				ret.emplace_back(
					EngineCard{
						.widget{.width = Size::Expand, .height = Size::Expand},
						.engine = loadout.engineInstanceKey ? &Store::engines.at(loadout.engineInstanceKey) : nullptr,
						.agentKey = widget->agentKey,
						.actions = EngineCard::Actions::showcase,
					}
				);

				std::visit(
					Utils::overloaded{
						[&](const Stats::Disc::Slotted &slotted) {
							for (const auto &partition: Disc::partitions) {
								const auto &arti = slotted.fromPartition(partition);
								if (!arti) {
									ret.emplace_back(Card{});
									continue;
								}

								ret.emplace_back(DiscCard{
									.widget{.width = Size::Expand, .height = Size::Expand},
									.disc = Store::discs.at(arti),
									.actions = DiscCard::Actions::showcase,
								});
							}
						},
						[&](const Stats::Disc::Theorycraft &) {
							for (const auto &_: Disc::partitions) {
								ret.emplace_back(Child{});
							}
						},
					},
					loadout.disc.equipped
				);

				return ret;
			}(),
		},
	};
}
