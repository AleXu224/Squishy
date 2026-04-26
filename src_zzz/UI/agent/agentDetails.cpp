
#include "agentDetails.hpp"

#include "UI/agent/agentDetailsSkill.hpp"
#include "UI/attributeToColor.hpp"
#include "UI/combo/comboDisplay.hpp"
#include "UI/disc/discCard.hpp"
#include "UI/engine/engineCard.hpp"
#include "UI/utils/card.hpp"
#include "UI/utils/masonry.hpp"
#include "agent/data.hpp"
#include "agentAnomalies.hpp"
#include "agentStats.hpp"
#include "disc/set.hpp"
#include "ranges"
#include "store.hpp"


#include "formula/stat.hpp"// IWYU pragma: keep

#include "modifiers/engine/displayStats.hpp"


#include "engine/data.hpp"
#include "utils/slotToCondition.hpp"
#include "widgets/column.hpp"
#include "widgets/expander.hpp"
#include "widgets/grid.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/themeOverride.hpp"
#include <map>


using namespace squi;

namespace {
	using MakeOptsRet = std::map<uint32_t, std::reference_wrapper<Option::Types>>;
	MakeOptsRet makeOptsSimple(std::unordered_map<uint32_t, Option::Types> &opts) {
		MakeOptsRet ret{};
		for (auto &[key, value]: opts) {
			ret.insert({key, std::ref(value)});
		}
		return ret;
	}

	MakeOptsRet makeOpts(const Option::EngineList &optList, std::unordered_map<uint32_t, Option::Types> &opts) {
		MakeOptsRet ret{};
		for (const auto &opt: optList) {
			const auto &key = std::visit(
				[](auto &&val) {
					return val.key.hash;
				},
				opt
			);
			if (opts.contains(key)) {
				ret.insert({key, std::ref(opts.at(key))});
			}
		}
		return ret;
	}
	MakeOptsRet makeDiscOpts(const Option::DiscList &opts, Option::TypesMap &map) {
		MakeOptsRet ret{};
		for (const auto &opt: opts) {
			std::visit(
				[&](auto &&val) {
					ret.insert({val.key, map.at(val.key)});
				},
				opt
			);
		}
		return ret;
	}

	Child makeMainContent(Agent::InstanceKey agentKey, std::optional<Team::InstanceKey> teamKey, Enemy::Key enemyKey) {
		auto &agent = ::Store::agents.at(agentKey);
		auto &team = teamKey ? ::Store::teams.at(teamKey.value()) : ::Store::defaultTeam;
		auto &enemy = ::Store::enemies.at(enemyKey);
		Formula::Context ctx{
			.source = agent.state,
			.active = agent.state,
			.team = team.stats,
			.enemy = enemy.stats,
		};

		Child anomalies = UI::AgentAnomalies{.ctx = ctx};

		Child combos = UI::ComboDisplay{
			.agentKey = agentKey,
			.ctx = ctx,
		};

		Child agentStats = UI::AgentStats{
			.ctx = ctx,
			.agentKey = agentKey,
		};

		std::variant<Agent::InstanceKey, Team::InstanceKey> keyParam = agentKey;
		if (teamKey.has_value()) keyParam = teamKey.value();

		auto teamOpts = makeOptsSimple(team.stats.options);

		std::vector<Node::Types> nodesPlaceholder;
		Child teamStats = UI::DetailsSkill{
			.name = "Resonance",
			.instanceKey = keyParam,
			.ctx = ctx,
			.nodes = nodesPlaceholder,
			.options = teamOpts,
			.modsGenerator = std::make_shared<UI::ModsGenerator>(),
		};

		MakeOptsRet engineOpts{};
		if (agent.state.loadout().engine) {
			engineOpts = makeOpts(agent.state.loadout().engine->data->data.opts, agent.state.options);
		}

		Child engineStats = nullptr;
		if (agent.state.loadout().engine) {
			engineStats = UI::DetailsSkill{
				.name = agent.state.loadout().engine->data->name,
				.subtitle = "Engine",
				.instanceKey = keyParam,
				.ctx = ctx,
				.nodes = agent.state.loadout().engine->data->data.nodes,
				.options = engineOpts,
				.modsSheet = std::ref(agent.state.loadout().engine->data->data.mods),
				.modsGenerator = std::make_shared<UI::DerivedModsGenerator>(Modifiers::Engine::displayStats()),
			};
		}

		std::optional<MakeOptsRet> discOpts1;
		if (agent.state.loadout().disc.bonus1.has_value())
			discOpts1 = makeDiscOpts(agent.state.loadout().disc.bonus1->bonusPtr->opts, agent.state.options);

		std::optional<MakeOptsRet> discOpts2;
		if (agent.state.loadout().disc.bonus2.has_value())
			discOpts2 = makeDiscOpts(agent.state.loadout().disc.bonus2->bonusPtr->opts, agent.state.options);

		std::optional<MakeOptsRet> discOpts3;
		if (agent.state.loadout().disc.bonus3.has_value())
			discOpts3 = makeDiscOpts(agent.state.loadout().disc.bonus3->bonusPtr->opts, agent.state.options);

		Child discStats1 = agent.state.loadout().disc.bonus1.has_value()
							 ? UI::DetailsSkill{
								   .name = std::string(agent.state.loadout().disc.bonus1->setPtr->name),
								   .subtitle = "Disc",
								   .instanceKey = keyParam,
								   .ctx = ctx,
								   .nodes = agent.state.loadout().disc.bonus1->bonusPtr->nodes,
								   .options = discOpts1,
								   .modsSheet = std::ref(agent.state.loadout().disc.bonus1->bonusPtr->mods),
							   }
							 : Child{};

		Child discStats2 = agent.state.loadout().disc.bonus2.has_value()
							 ? UI::DetailsSkill{
								   .name = std::string(agent.state.loadout().disc.bonus2->setPtr->name),
								   .subtitle = "Disc",
								   .instanceKey = keyParam,
								   .ctx = ctx,
								   .nodes = agent.state.loadout().disc.bonus2->bonusPtr->nodes,
								   .options = discOpts2,
								   .modsSheet = std::ref(agent.state.loadout().disc.bonus2->bonusPtr->mods),
							   }
							 : Child{};
		Child discStats3 = agent.state.loadout().disc.bonus3.has_value()
							 ? UI::DetailsSkill{
								   .name = std::string(agent.state.loadout().disc.bonus3->setPtr->name),
								   .subtitle = "Disc",
								   .instanceKey = keyParam,
								   .ctx = ctx,
								   .nodes = agent.state.loadout().disc.bonus3->bonusPtr->nodes,
								   .options = discOpts3,
								   .modsSheet = std::ref(agent.state.loadout().disc.bonus3->bonusPtr->mods),
							   }
							 : Child{};

		Child leftColumn = Column{
			.spacing = 4.f,
			.children{
				teamStats,
				anomalies,
				combos,
				engineStats,
				discStats1,
				discStats2,
				discStats3,
			},
		};

		Children normalColumnChildren{};
		Children skillColumnChildren{};
		Children burstColumnChildren{};
		Children otherColumnChildren{};

		std::vector<std::map<uint32_t, std::reference_wrapper<Option::Types>>> agentOpts{};
		for (auto &optPtr: Option::AgentList::getMembers()) {
			const auto &optList = std::invoke(optPtr, agent.state.stats.data.data->opts);
			agentOpts.emplace_back(makeOpts(optList, agent.state.options));
		}
		std::vector<std::reference_wrapper<const std::vector<Node::Types>>> nodes{};
		for (auto &nodePtr: Node::AgentList::getMembers()) {
			nodes.emplace_back(std::invoke(nodePtr, agent.state.stats.data.data->nodes));
		}

		for (const auto &[nodeWrapper, options, slot]: std::views::zip(nodes, agentOpts, Node::agentSlots)) {
			auto name = Utils::Stringify(slot);
			auto condition = Utils::slotToCondition(slot);
			const auto &nodes = nodeWrapper.get();
			if (nodes.empty() && options.empty()) continue;

			Child ret = UI::DetailsSkill{
				.name = name,
				.instanceKey = keyParam,
				.ctx = ctx,
				.nodes = nodes,
				.options = options,
				.displayCondition = condition,
			};
			switch (slot) {
				case Node::AgentSlot::basic:
					normalColumnChildren.emplace_back(ret);
					break;
				case Node::AgentSlot::dodge:
				case Node::AgentSlot::dashAttack:
				case Node::AgentSlot::dodgeCounter:
				case Node::AgentSlot::special:
				case Node::AgentSlot::exspecial:
					skillColumnChildren.emplace_back(ret);
					break;
				case Node::AgentSlot::assist:
				case Node::AgentSlot::quickAssist:
				case Node::AgentSlot::defensiveAssist:
				case Node::AgentSlot::evasiveAssist:
				case Node::AgentSlot::assistFollowup:
				case Node::AgentSlot::chain:
				case Node::AgentSlot::ultimate:
					burstColumnChildren.emplace_back(ret);
					break;
				case Node::AgentSlot::core:
				case Node::AgentSlot::mindscape1:
				case Node::AgentSlot::mindscape2:
				case Node::AgentSlot::mindscape3:
				case Node::AgentSlot::mindscape4:
				case Node::AgentSlot::mindscape5:
				case Node::AgentSlot::mindscape6:
					otherColumnChildren.emplace_back(ret);
					break;
			}
		}

		normalColumnChildren.emplace_back(UI::DetailsSkill{
			.name = "Incoming Team Buffs",
			.instanceKey = keyParam,
			.ctx = ctx,
			.modsGenerator = std::make_shared<UI::DerivedModsGenerator>(Modifiers::team()),
		});

		Child normalColumn = Column{.spacing = 4.f, .children = normalColumnChildren};
		Child skillColumn = Column{.spacing = 4.f, .children = skillColumnChildren};
		Child burstColumn = Column{.spacing = 4.f, .children = burstColumnChildren};
		Child otherColumn = Column{.spacing = 4.f, .children = otherColumnChildren};

		Children mainContent{
			agentStats,
			leftColumn,
			normalColumn,
			skillColumn,
			burstColumn,
			otherColumn,
		};

		return Column{
			.widget{
				.height = Size::Shrink,
				.sizeConstraints = BoxConstraints{
					.maxWidth = 1520.f,
				},
			},
			.spacing = 4.f,
			.children{
				UI::Masonry{
					.columnCount = UI::Masonry::MinSize{250.f},
					.spacing = 4.f,
					.children = mainContent,
				},
			},
		};
	}

	struct EquipmentExpander : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;

		[[nodiscard]] Child build(const Element &) const {
			auto &agent = ::Store::agents.at(agentKey);

			return Expander{
				.widget = widget,
				.title = "Equipment",
				.content = Grid{
					.widget{
						.padding = Padding{8.f, 8.f},
					},
					.columnCount = UI::Grid::MinSize{250.f},
					.spacing = 4.f,
					.children = [&]() {
						Children ret;
						ret.emplace_back(UI::EngineCard{
							.engine = agent.state.loadout().engineInstanceKey//
										? &::Store::engines.at(agent.state.loadout().engineInstanceKey)
										: nullptr,
							.agentKey = agentKey,
							.actions = UI::EngineCard::Actions::agent,
						});
						std::visit(
							Utils::overloaded{
								[&](const Stats::Disc::Slotted &discs) {
									for (const auto &partition: Disc::partitions) {
										const auto &key = discs.fromPartition(partition);
										if (!key) {
											ret.emplace_back(UI::Card{});
											continue;
										}
										auto &disc = ::Store::discs.at(key);
										ret.emplace_back(UI::DiscCard{
											.disc = disc,
											.actions = UI::DiscCard::Actions::agent,
										});
									}
								},
								[&](const Stats::Disc::Theorycraft &stats) {
									// FIXME: add display for theorycraft display
								},
							},
							agent.state.loadout().disc.equipped
						);
						return ret;
					}(),
				},
			};
		}
	};
}// namespace

squi::core::Child UI::AgentDetails::State::build(const Element &element) {
	auto &agent = Store::agents.at(widget->agentKey);
	auto theme = Theme::of(element);
	theme.accent = Utils::attributeToColor(agent.state.stats.base.attribute);
	return ThemeOverride{
		.theme = theme,
		.child = ScrollView{
			.scrollWidget{
				.padding = 4.f,
			},
			.alignment = Flex::Alignment::center,
			.spacing = 4.f,
			.children{
				EquipmentExpander{
					.widget{
						.sizeConstraints = BoxConstraints{
							.maxWidth = 1520.f,
						},
					},
					.agentKey = widget->agentKey,
				},
				makeMainContent(widget->agentKey, widget->teamKey, widget->enemyKey),
				// FIXME: optimization
				// std::visit(//
				// 	Utils::overloaded{
				// 		[&](const Stats::Disc::Slotted &lotted) -> Child {
				// 			return UI::Optimization{
				// 				.agentKey = widget->agentKey,
				// 				.teamKey = widget->teamKey,
				// 				.enemyKey = widget->enemyKey,
				// 			};
				// 		},
				// 		[&](const Stats::Disc::Theorycraft &theorycraft) -> Child {
				// 			return UI::TCOptimization{
				// 				.agentKey = widget->agentKey,
				// 				.teamKey = widget->teamKey,
				// 				.enemyKey = widget->enemyKey,
				// 			};
				// 		},
				// 	},
				// 	agent.state.loadout().disc.equipped
				// ),
			},
		},
	};
}
