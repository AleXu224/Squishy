#include "optimization.hpp"
#include "UI/combo/nodePicker.hpp"
#include "optimization/optimize.hpp"
#include "optimizationMainStatChooser.hpp"
#include "optimizationResult.hpp"
#include "optimizationSetChooser.hpp"
#include "store.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/expander.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/toggleSwitch.hpp"

#include "optimization/solution.hpp"


using namespace squi;

squi::core::Child UI::Optimization::State::build(const Element &element) {
	auto &agent = ::Store::agents.at(widget->agentKey);
	auto &team = widget->teamKey ? ::Store::teams.at(widget->teamKey.value()) : ::Store::defaultTeam;
	auto &enemy = ::Store::enemies.at(widget->enemyKey);
	Formula::Context ctx{
		.source = agent.state,
		.origin = agent.state,
		.team = team.stats,
		.enemy = enemy.stats,
	};

	return Container{
		.widget = widget->widget,
		.child = Column{
			.widget{
				.alignment = Alignment::TopCenter,
				.sizeConstraints = BoxConstraints{
					.maxWidth = 1520.f,
				},
			},
			.crossAxisAlignment = Column::Alignment::center,
			.spacing = 3.f,
			.children{
				Expander{
					.title = "Enabled sets and bonuses",
					.subtitle = "Select which sets and bonuses will be used in the optimization",
					.action{
						Button{
							.theme = Button::Theme::Standard(),
							.onClick = [this, storage = agent.optimizationOptions, &agent = agent, ctx = ctx]() {
								Navigator::of(*this->element).pushOverlay(OptimizationSetChooser{
									.agent = agent,
									.ctx = ctx,
									.twoPcSets = storage->twoPcSets,
									.fourPcSets = storage->fourPcSets,
								});
							},
							.child = "Configure",
						},
					},
				},
				Expander{
					.title = "Enabled main stats",
					.subtitle = "Choose which main stats will be used in the optimization",
					.content = OptimizationMainStatChooser{
						.partition4MainStats = agent.optimizationOptions->partition4MainStats,
						.partition5MainStats = agent.optimizationOptions->partition5MainStats,
						.partition6MainStats = agent.optimizationOptions->partition6MainStats,
					},
				},
				Expander{
					.title = "Use equipped discs",
					.subtitle = "Whether to allow to usage of discs currently equipped on other agents",
					.action{
						ToggleSwitch{
							.active = agent.optimizationOptions->useEquippedDiscs,
							.onToggle = [this, storage = agent.optimizationOptions](bool active) {
								setState([&]() {
									storage->useEquippedDiscs = active;
								});
							},
						},
					},
				},
				Expander{
					.title = "Optimize",
					.action{
						Row{
							.widget{
								.width = Size::Wrap,
							},
							.crossAxisAlignment = Row::Alignment::center,
							.children{
								Button{
									.theme = []() {
										auto ret = Button::Theme::Standard();
										ret.forAll([](Button::Style &style) {
											style.borderRadius = style.borderRadius.withRight(0.f);
										});
										return ret;
									}(),
									.onClick = [this, storage = agent.optimizationOptions, ctx = ctx]() {
										Navigator::of(*this->element).pushOverlay(NodePicker{
											.agentKey = widget->agentKey,
											.enableCombos = true,
											.ctx = ctx,
											.onSelect = [this, storage](const Combo::Source::Types &source) {
												setState([&]() {
													storage->nodeSource = source;
												});
											},
										});
									},
									.child = agent.optimizationOptions->nodeSource.has_value()//
											   ? std::visit(                                  //
													 [&](auto &&node) -> std::string {
														 return Node::getName(node.resolve({}).data, ctx);
													 },
													 agent.optimizationOptions->nodeSource.value()
												 )
											   : "Select optimization target",
								},
								Button{
									.theme = [&]() {
										auto ret = Button::Theme::Accent(element);
										ret.forAll([](Button::Style &style) {
											style.borderRadius = style.borderRadius.withLeft(0.f);
										});
										return ret;
									}(),
									.onClick = [this, ctx, &agent, storage = agent.optimizationOptions]() {
										if (!storage->nodeSource.has_value()) return;
										auto &&node = std::visit(
											[&](auto &&node) {
												return node
													.resolve({})
													.formula;
											},
											storage->nodeSource.value()
										);
										auto [twoPc, fourPc] = storage->makeEnabledSets();
										::Optimization::Optimization optimization{
											.agent = agent,
											.ctx = ctx,
											.optimizedNode = node,
											.options = *storage,
										};
										auto solutions = optimization.optimize();
										setState([&]() {
											this->solutions = solutions;
										});
									},
									.child = "Optimize",
								},
							},
						},
					},
				},
				Column{
					.spacing = 3.f,
					.children = [this]() {
						Children ret;
						uint32_t counter = 1;
						for (const auto &solution: solutions.solutions) {
							if (solution.score <= 0.f) continue;
							ret.emplace_back(OptimizationResult{
								.agentKey = widget->agentKey,
								.solution = solution,
								.entryIndex = counter++,
							});
						}
						return ret;
					}(),
				},
			},
		},
	};
}
