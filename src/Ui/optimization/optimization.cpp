#include "optimization.hpp"
#include "Ui/combo/nodePicker.hpp"
#include "optimization/optimize.hpp"
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
		.child = Column{
			.widget{
				.alignment = Alignment::CenterLeft,
				.sizeConstraints = BoxConstraints{
					.maxWidth = 1520.f,
				},
			},
			.crossAxisAlignment = Column::Alignment::center,
			.spacing = 3.f,
			.children{
				Expander{
					.title = "Enable 3 rainbow",
					.subtitle = "Having a 2pc set and three other artifacts that don't make up any set. Disabling this will drastically speed up the optimization",
					.action{
						ToggleSwitch{
							.active = character.optimizationOptions->threeRainbow,
							.onToggle = [this, storage = character.optimizationOptions](bool active) {
								setState([&]() {
									storage->threeRainbow = active;
								});
							},
						},
					},
				},
				Expander{
					.title = "Enable 5 rainbow",
					.subtitle = "Having no 2pc or 4pc set. Disabling this will drastically speed up the optimization",
					.action{
						ToggleSwitch{
							.active = character.optimizationOptions->fiveRainbow,
							.onToggle = [this, storage = character.optimizationOptions](bool active) {
								setState([&]() {
									storage->fiveRainbow = active;
								});
							},
						},
					},
				},
				Expander{
					.title = "Enabled sets and bonuses",
					.subtitle = "Select which sets and bonuses will be used in the optimization",
					.action{
						Button{
							.theme = Button::Theme::Standard(),
							.onClick = [this, storage = character.optimizationOptions, &character = character, ctx = ctx]() {
								Navigator::of(*this->element).pushOverlay(OptimizationSetChooser{
									.character = character,
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
					.title = "Optimize",
					.action{
						Row{
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
										auto [twoPc, fourPc] = storage->makeEnabledSets();
										::Optimization::Optimization optimization{
											.character = character,
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
								.characterKey = widget->characterKey,
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
