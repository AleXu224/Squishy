#include "tcOptimization.hpp"
#include "Ui/combo/nodePicker.hpp"
#include "align.hpp"
#include "button.hpp"
#include "column.hpp"
#include "container.hpp"
#include "expander.hpp"
#include "optimization/optimizeTc.hpp"
#include "rebuilder.hpp"
#include "row.hpp"
#include "store.hpp"
#include "tcOptimizationSettings.hpp"
#include "theme.hpp"


using namespace squi;

UI::TCOptimization::operator squi::Child() const {
	auto &character = ::Store::characters.at(characterKey);
	auto &team = teamKey ? ::Store::teams.at(teamKey.value()) : ::Store::defaultTeam;
	auto &enemy = ::Store::enemies.at(enemyKey);
	Formula::Context ctx{
		.source = character.state,
		.active = character.state,
		.team = team.stats,
		.enemy = enemy.stats,
	};

	VoidObservable applyNewSettingsEvent;

	auto theme = ThemeManager::getTheme();

	return Container{
		.child = Align{
			.yAlign = 0.f,
			.child = Column{
				.widget{
					.sizeConstraints{
						.maxWidth = 1520.f,
					},
				},
				.alignment = squi::Column::Alignment::center,
				.spacing = 3.f,
				.children{
					Expander{
						.heading = "Optimize",
						.actions{
							Row{
								.children{
									Button{
										.text = character.optimizationOptions->nodeSource.has_value()//
												  ? std::visit([](auto &&node) {
														return node.resolve({}).name;
													},
															   character.optimizationOptions->nodeSource.value())
												  : "Select optimization target",
										.style = []() {
											auto ret = ButtonStyle::Standard();
											ret.borderRadius = ret.borderRadius.withRight(0.f);
											return ret;
										}(),
										.onClick = [storage = character.optimizationOptions, characterKey = characterKey, ctx = ctx, theme](GestureDetector::Event event) {
											auto _ = ThemeManager::pushTheme(theme);

											event.widget.addOverlay(NodePicker{
												.characterKey = characterKey,
												.enableCombos = true,
												.ctx = ctx,
												.onSelect = [storage, &w = event.widget](const Combo::Source::Types &source) {
													storage->nodeSource = source;
													auto node = std::visit(
														[](auto &&node) {
															return node.resolve({});
														},
														source
													);
													Button::State::updateText.of(w).notify(std::string{node.name});
												},
											});
										},
									},
									Button{
										.text = "Optimize",
										.style = []() {
											auto ret = ButtonStyle::Accent();
											ret.borderRadius = ret.borderRadius.withLeft(0.f);
											return ret;
										}(),
										.onClick = [ctx, &character, storage = character.optimizationOptions, applyNewSettingsEvent](auto) {
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
											for (const auto &stat: Stats::subStats) {
												loadout.fromStat(stat) = solution.artifacts.fromStat(stat);
											}
											loadout.updateStats();
											character.state.loadout().artifact.refreshStats();
											character.updateEvent.notify();

											applyNewSettingsEvent.notify();
										},
									},
								},
							},
						},
					},
					Rebuilder{
						.rebuildEvent = applyNewSettingsEvent,
						.buildFunc = [ctx = ctx, &character = character, theme]() -> Child {
							auto _ = ThemeManager::pushTheme(theme);

							return TCOptimizationSettings{
								.character = character,
								.ctx = ctx,
							};
						},
					},
				},
			},
		},
	};
}
