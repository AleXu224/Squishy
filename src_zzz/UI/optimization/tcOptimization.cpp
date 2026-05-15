#include "tcOptimization.hpp"
#include "UI/combo/nodePicker.hpp"
#include "optimization/optimizeTc.hpp"
#include "store.hpp"
#include "tcOptimizationSettings.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/expander.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"


using namespace squi;
squi::core::Child UI::TCOptimization::State::build(const Element &element) {
	auto &agent = ::Store::agents.at(widget->agentKey);
	auto &team = widget->teamKey ? ::Store::teams.at(widget->teamKey.value()) : ::Store::defaultTeam;
	auto &enemy = ::Store::enemies.at(widget->enemyKey);
	Formula::Context ctx{
		.source = agent.state,
		.active = agent.state,
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
										auto [twoPc, fourPc] = agent.state.loadout().disc.getTheorycraft().getSets();

										::Optimization::OptionsTc opts{
											.nodeSource = storage->nodeSource,
											.twoPcSet = twoPc,
											.fourPcSet = fourPc,
										};

										::Optimization::TCOptimization optimization{
											.agent = agent,
											.ctx = ctx,
											.optimizedNode = node,
											.options = opts,
										};
										auto solution = optimization.optimize();

										auto &loadout = agent.state.loadout().disc.getTheorycraft();
										setState([&]() {
											for (const auto &stat: Stats::subStats) {
												loadout.fromStat(stat) = solution.discs.fromStat(stat);
											}
										});
										loadout.updateStats();
										agent.state.loadout().disc.refreshStats();
										agent.updateEvent.notify();
									},
									.child = "Optimize",
								},
							},
						},
					},
				},
				TCOptimizationSettings{
					.agent = agent,
					.ctx = ctx,
				},
			},
		},
	};
}
