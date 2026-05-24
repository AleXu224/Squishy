#include "optimization.hpp"
#include "UI/combo/nodePicker.hpp"
#include "optimization/optimize.hpp"
#include "optimization/optimizeUpgrade.hpp"
#include "optimizationMainStatChooser.hpp"
#include "optimizationResult.hpp"
#include "optimizationResultUpgrade.hpp"
#include "optimizationSetChooser.hpp"
#include "store.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/expander.hpp"
#include "widgets/grid.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/slider.hpp"
#include "widgets/toggleSwitch.hpp"

#include "optimization/solution.hpp"


using namespace squi;

squi::core::Child UI::Optimization::State::build(const Element &element) {
	auto &character = ::Store::characters.at(widget->characterKey);
	auto &team = widget->teamKey ? ::Store::teams.at(widget->teamKey.value()) : ::Store::defaultTeam;
	auto &enemy = ::Store::enemies.at(widget->enemyKey);
	Formula::Context ctx{
		.source = character.state,
		.origin = character.state,
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
					.title = "Enabled main stats",
					.subtitle = "Choose which main stats will be used in the optimization",
					.content = OptimizationMainStatChooser{
						.sandsMainStats = character.optimizationOptions->sandsMainStats,
						.gobletMainStats = character.optimizationOptions->gobletMainStats,
						.circletMainStats = character.optimizationOptions->circletMainStats,
					},
				},
				Expander{
					.title = "Minimum artifact level",
					.subtitle = "Set the minimum level of artifacts to consider in the optimization",
					.action{
						Slider{
							.widget{
								.width = 250.f,
							},
							.minValue = 0.f,
							.maxValue = 15.f,
							.value = static_cast<float>(character.optimizationOptions->minLevel),
							.ticks = std::views::iota(0, 16) | std::ranges::to<std::vector<float>>(),
							.onChange = [this, storage = character.optimizationOptions](float value) {
								setState([&]() {
									storage->minLevel = static_cast<uint8_t>(value);
								});
							},
						},
					},
				},
				Expander{
					.title = "Use equipped artifacts",
					.subtitle = "Whether to allow to usage of artifacts currently equipped on other characters",
					.action{
						ToggleSwitch{
							.active = character.optimizationOptions->useEquippedArtifacts,
							.onToggle = [this, storage = character.optimizationOptions](bool active) {
								setState([&]() {
									storage->useEquippedArtifacts = active;
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
													 [&](auto &&node) -> std::string {
														 return Node::getName(node.resolve({}).data, ctx);
													 },
													 character.optimizationOptions->nodeSource.value()
												 )
											   : "Select optimization target",
								},
								Button{
									.theme = [&]() {
										auto ret = Button::Theme::Accent(element);
										ret.forAll([](Button::Style &style) {
											style.borderRadius = 0.f;
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
								Button{
									.theme = [&]() {
										auto ret = Button::Theme::Standard();
										ret.forAll([](Button::Style &style) {
											style.borderRadius = style.borderRadius.withLeft(0.f);
										});
										return ret;
									}(),
									.onClick = [ctx, &character, storage = character.optimizationOptions, this]() {
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
										::Optimization::UpgradeOptimization optimization{
											.character = character,
											.ctx = ctx,
											.optimizedNode = node,
											.options{
												.nodeSource = storage->nodeSource,
											},
										};
										auto solutions = optimization.optimize();
										setState([&]() {
											this->solutions = solutions;
										});
									},
									.child = "Upgrade artifacts",
								},
							},
						},
					},
				},
				std::visit(//
					Utils::overloaded{
						[&](const ::Optimization::Solutions &solutions) -> Child {
							return Column{
								.spacing = 3.f,
								.children = [&]() {
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
							};
						},
						[&](const ::Optimization::SolutionsUpgrade &solutions) -> Child {
							return Grid{
								.columnCount = Grid::MinSize{300.f},
								.spacing = 3.f,
								.children = [&]() {
									Children ret;
									uint32_t counter = 1;
									for (const auto &solution: solutions.solutions) {
										if (!solution.artifact) continue;
										ret.emplace_back(OptimizationResultUpgrade{
											.characterKey = widget->characterKey,
											.solution = solution,
											.entryIndex = counter++,
										});
									}
									return ret;
								}(),
							};
						},
					},
					solutions
				),
			},
		},
	};
}
