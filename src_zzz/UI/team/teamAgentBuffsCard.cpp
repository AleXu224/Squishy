#include "teamAgentBuffsCard.hpp"
#include "UI/agent/agentCardBanner.hpp"
#include "UI/option/toggleOption.hpp"
#include "UI/option/valueListOption.hpp"
#include "UI/utils/displayCard.hpp"

#include "agent/data.hpp"
#include "engine/data.hpp"
#include "store.hpp"


using namespace squi;

squi::core::Child UI::TeamAgentBuffsCard::State::build(const Element &element) {
	auto &agent = widget->agent;
	auto &team = widget->team;

	return UI::DisplayCard{
		.title = std::string(agent.state.stats.data.name),
		.children{
			UI::AgentCardBanner{
				.agentKey = agent.instanceKey,
			},
		},
		.footer = [&agent = agent, &team = team]() {
			Children ret{};

			auto activeAgent = team.stats.agents.at(team.stats.activeAgentIndex);
			Formula::Context ctx{
				.source = agent.state,
				.prevSource = activeAgent ? activeAgent->state : agent.state,
				.active = agent.state,
				.team = team.stats,
				.enemy = Store::enemies.at(0).stats,
			};

			auto parseOption = [&](Option::Types &option) {
				std::visit(
					Utils::overloaded{
						[&](Option::Boolean &opt) {
							if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(ctx)) return;
							ret.emplace_back(UI::ToggleOption{
								.option = opt,
								.instanceKey = team.instanceKey,
								.ctx = ctx,
							});
						},
						[&](Option::ValueList &opt) {
							if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(ctx)) return;
							ret.emplace_back(UI::ValueListOption{
								.option = opt,
								.instanceKey = team.instanceKey,
								.ctx = ctx,
							});
						},
					},
					option
				);
			};
			auto isTeamBuff = [](const Option::Types &option) {
				return std::visit(
					[](auto &&opt) {
						return opt.teamBuff;
					},
					option
				);
			};

			if (agent.state.loadout().engine) {
				for (auto &opt: agent.state.loadout().engine->data->data.opts) {
					if (!isTeamBuff(opt)) continue;
					parseOption(agent.state.options.at(
						std::visit(
							[](auto &&opt) {
								return opt.key.hash;
							},
							opt
						)
					));
				}
			}

			if (agent.state.loadout().disc.bonus1.has_value()) {
				for (auto &opt: agent.state.loadout().disc.bonus1->bonusPtr->opts) {
					if (!isTeamBuff(opt)) continue;
					parseOption(agent.state.options.at(
						std::visit(
							[](auto &&opt) {
								return opt.key.hash;
							},
							opt
						)
					));
				}
			}
			if (agent.state.loadout().disc.bonus2.has_value()) {
				for (auto &opt: agent.state.loadout().disc.bonus2->bonusPtr->opts) {
					if (!isTeamBuff(opt)) continue;
					parseOption(agent.state.options.at(
						std::visit(
							[](auto &&opt) {
								return opt.key.hash;
							},
							opt
						)
					));
				}
			}

			for (const auto &[optPtr, condition]: Option::AgentList::getMembersAndConditions()) {
				if (!condition.eval(ctx)) continue;
				auto &optList = std::invoke(optPtr, agent.state.stats.data.data->opts);
				for (auto &optionData: optList) {
					bool isTeamBuff = std::visit(
						[](auto &&opt) {
							return opt.teamBuff;
						},
						optionData
					);
					if (!isTeamBuff) continue;
					parseOption(agent.state.options.at(
						std::visit(
							[](auto &&opt) {
								return opt.key.hash;
							},
							optionData
						)
					));
				}
			}

			return ret;
		}(),
	};
}
