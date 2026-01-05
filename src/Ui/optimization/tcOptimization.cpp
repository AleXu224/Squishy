#include "tcOptimization.hpp"
#include "Ui/combo/nodePicker.hpp"
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
	auto &character = ::Store::characters.at(widget->characterKey);
	auto &team = widget->teamKey ? ::Store::teams.at(widget->teamKey.value()) : ::Store::defaultTeam;
	auto &enemy = ::Store::enemies.at(widget->enemyKey);
	Formula::Context ctx{
		.source = character.state,
		.active = character.state,
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
									.onClick = [this, storage = character.optimizationOptions, ctx = ctx]() {
										Navigator::of(*this->element).pushOverlay(NodePicker{
											.characterKey = widget->characterKey,
											.enableCombos = true,
											.ctx = ctx,
											.onSelect = [this, storage](const Combo::Source::Types &source) {
												setState([&]() {
													storage->nodeSource = source;
												});
											},
										});
									},
									.child = character.optimizationOptions->nodeSource.has_value()//
											   ? std::visit(                                      //
													 [](auto &&node) -> std::string {
														 return std::string(node.resolve({}).name);
													 },
													 character.optimizationOptions->nodeSource.value()
												 )
											   : "Select optimization target",
								},
								Button{
									.theme = []() {
										auto ret = Button::Theme::Accent();
										ret.forAll([](Button::Style &style) {
											style.borderRadius = style.borderRadius.withLeft(0.f);
										});
										return ret;
									}(),
									.onClick = [this, ctx, &character, storage = character.optimizationOptions]() {
										if (!storage->nodeSource.has_value()) return;
										auto &&node = std::visit(
											[&](auto &&node) {
												return node
													.resolve({})
													.formula;
											},
											storage->nodeSource.value()
										);
										auto [twoPc, fourPc] = character.state.loadout().artifact.getTheorycraft().getSets();
										::Optimization::TCOptimization optimization{
											.character = character,
											.ctx = ctx,
											.optimizedNode = node,
											.options{
												.nodeSource = storage->nodeSource,
												.twoPcSet = twoPc,
												.fourPcSet = fourPc,
											},
										};
										auto solution = optimization.optimize();

										auto &loadout = character.state.loadout().artifact.getTheorycraft();
										setState([&]() {
											for (const auto &stat: Stats::subStats) {
												loadout.fromStat(stat) = solution.artifacts.fromStat(stat);
											}
										});
										loadout.updateStats();
										character.state.loadout().artifact.refreshStats();
										character.updateEvent.notify();
									},
									.child = "Optimize",
								},
							},
						},
					},
				},
				TCOptimizationSettings{
					.character = character,
					.ctx = ctx,
				},
			},
		},
	};
}
