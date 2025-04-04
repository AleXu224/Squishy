#include "optimization.hpp"
#include "Ui/combo/nodePicker.hpp"
#include "align.hpp"
#include "button.hpp"
#include "column.hpp"
#include "container.hpp"
#include "expander.hpp"
#include "optimization/optimize.hpp"
#include "optimizationResult.hpp"
#include "optimizationSetChooser.hpp"
#include "row.hpp"
#include "store.hpp"
#include "widgets/toggleSwitch.hpp"

#include "optimization/solution.hpp"


using namespace squi;

namespace {
	struct OptimizationStorage {
		std::optional<Combo::Source::Types> nodeSource;
		std::unordered_map<Artifact::SetKey, bool> twoPcSets{};
		std::unordered_map<Artifact::SetKey, bool> fourPcSets{};
		bool threeRainbow = true;
		bool fiveRainbow = true;

		auto makeEnabledSets() const {
			std::vector<Artifact::SetKey> retTwoPcSets{};
			std::vector<Artifact::SetKey> retFourPcSets{};

			for (const auto &[key, val]: twoPcSets) {
				if (val) retTwoPcSets.emplace_back(key);
			}
			for (const auto &[key, val]: fourPcSets) {
				if (val) retFourPcSets.emplace_back(key);
			}

			return std::pair{retTwoPcSets, retFourPcSets};
		}
	};
}// namespace

UI::Optimization::operator squi::Child() const {
	auto storage = std::make_shared<OptimizationStorage>();

	for (const auto &[key, set]: Artifact::sets) {
		storage->twoPcSets[key] = true;
		storage->fourPcSets[key] = true;
	}

	auto &character = ::Store::characters.at(characterKey);
	auto &team = teamKey ? ::Store::teams.at(teamKey.value()) : ::Store::defaultTeam;
	auto &enemy = ::Store::enemies.at(enemyKey);
	Formula::Context ctx{
		.source = character.loadout,
		.active = character.loadout,
		.team = team.stats,
		.enemy = enemy.stats,
	};

	Observable<::Optimization::Solutions> solutionsEvent;

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
						.heading = "Enable 3 rainbow",
						.caption = "Having a 2pc set and three other artifacts that don't make up any set. Disabling this will drastically speed up the optimization",
						.actions{
							ToggleSwitch{
								.active = storage->threeRainbow,
								.onSwitch = [storage](bool active) {
									storage->threeRainbow = active;
								},
							},
						},
					},
					Expander{
						.heading = "Enable 5 rainbow",
						.caption = "Having no 2pc or 4pc set. Disabling this will drastically speed up the optimization",
						.actions{
							ToggleSwitch{
								.active = storage->fiveRainbow,
								.onSwitch = [storage](bool active) {
									storage->fiveRainbow = active;
								},
							},
						},
					},
					Expander{
						.heading = "Enabled sets and bonuses",
						.caption = "Select which sets and bonuses will be used in the optimization",
						.actions{
							Button{
								.text = "Configure",
								.style = ButtonStyle::Standard(),
								.onClick = [storage, &character = character, ctx = ctx](GestureDetector::Event event) {
									event.widget.addOverlay(OptimizationSetChooser{
										.character = character,
										.ctx = ctx,
										.twoPcSets = storage->twoPcSets,
										.fourPcSets = storage->fourPcSets,
									});
								},
							},
						},
					},
					Expander{
						.heading = "Optimize",
						.actions{
							Row{
								.children{
									Button{
										.text = "Select optimization target",
										.style = []() {
											auto ret = ButtonStyle::Standard();
											ret.borderRadius = ret.borderRadius.withRight(0.f);
											return ret;
										}(),
										.onClick = [storage, characterKey = characterKey, ctx = ctx](GestureDetector::Event event) {
											event.widget.addOverlay(NodePicker{
												.characterKey = characterKey,
												.enableCombos = true,
												.ctx = ctx,
												.onSelect = [storage, &w = event.widget](const Combo::Source::Types &source) {
													storage->nodeSource = source;
													auto node = std::visit(
														[](auto &&node) {
															return node.resolve();
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
										.onClick = [ctx, &character, storage, solutionsEvent](auto) {
											if (!storage->nodeSource.has_value()) return;
											auto &&node = std::visit(
												[](auto &&node) {
													return node.resolve().formula;
												},
												storage->nodeSource.value()
											);
											auto [twoPc, fourPc] = storage->makeEnabledSets();
											::Optimization::Optimization optimization{
												.character = character,
												.ctx = ctx,
												.optimizedNode = node,
												.threeRainbow = storage->threeRainbow,
												.fiveRainbow = storage->fiveRainbow,
												.enabledTwoPiece = twoPc,
												.enabledFourPiece = fourPc,
											};
											solutionsEvent.notify(optimization.optimize());
										},
									},
								},
							},
						},
					},
					Column{
						.widget{
							.onInit = [solutionsEvent, characterKey = characterKey](Widget &w) {
								observe(w, solutionsEvent, [&w, characterKey](::Optimization::Solutions solutions) {
									Children ret;
									uint32_t counter = 1;
									for (const auto &solution: solutions.solutions) {
										if (solution.score <= 0.f) continue;
										ret.emplace_back(OptimizationResult{
											.characterKey = characterKey,
											.solution = solution,
											.entryIndex = counter++,
										});
									}
									w.setChildren(ret);
								});
							},
						},
						.spacing = 3.f,
					},
				},
			},
		},
	};
}
