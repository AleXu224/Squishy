#include "optimizationSetChooser.hpp"
#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"

#include "Ui/utils/card.hpp"
#include "artifact/key.hpp"
#include "artifact/sets.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/dialog.hpp"
#include "widgets/grid.hpp"
#include "widgets/image.hpp"
#include "widgets/row.hpp"
#include "widgets/text.hpp"
#include "widgets/toggleButton.hpp"


using namespace squi;

namespace {
	struct OptimizationSetChooserItem : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Artifact::SetKey set;
		Character::Instance &character;
		Formula::Context ctx;
		std::unordered_map<Artifact::SetKey, bool> &twoPcSets;
		std::unordered_map<Artifact::SetKey, bool> &fourPcSets;

		struct State : WidgetState<OptimizationSetChooserItem> {
			Child build(const Element &element) override {
				const auto &data = Artifact::sets.at(widget->set);

				Child header = Row{
					.widget{.height = Size::Shrink},
					.crossAxisAlignment = Row::Alignment::center,
					.children{
						Image{
							.widget{
								.width = 64.f,
								.height = 64.f,
							},
							.fit = Image::Fit::contain,
							.image = ImageProvider::fromFile(
								std::format(
									"assets/Artifacts/{}/{}.png",
									Artifact::sets.at(widget->set).name,
									Utils::Stringify(Artifact::Slot::flower)
								)
							)
						},
						Container{
							.widget{.height = Size::Shrink},
							.child = Text{
								.text = std::string(data.name),
								.lineWrap = true,
							},
						},
					},
				};

				Child opts = Column{
					.widget{
						.height = Size::Shrink,
					},
					.spacing = 4.f,
					.children = [&]() {
						Children ret;

						auto instanceKey = widget->character.instanceKey;

						auto opt_lists = std::vector{
							std::ref(data.data.twoPc.opts),
							std::ref(data.data.fourPc.opts),
						};

						for (auto &opt_list: opt_lists) {
							for (auto &opt: opt_list.get()) {
								std::visit(
									Utils::overloaded{
										[&](Option::Boolean &opt) {
											if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(widget->ctx)) return;
											ret.emplace_back(UI::ToggleOption{
												.option = opt,
												.instanceKey = instanceKey,
												.ctx = widget->ctx,
											});
										},
										[&](Option::ValueList &opt) {
											if (opt.displayCondition.hasValue() && !opt.displayCondition.eval(widget->ctx)) return;
											ret.emplace_back(UI::ValueListOption{
												.option = opt,
												.instanceKey = instanceKey,
												.ctx = widget->ctx,
											});
										},
									},
									widget->character.state.options.at(
										std::visit(
											[](auto &&val) {
												return val.key;
											},
											opt
										)
									)
								);
							}
						}

						return ret;
					}(),
				};

				Child footer = Row{
					.widget{
						.height = Size::Shrink,
					},
					.children{
						ToggleButton{
							.widget{.width = Size::Expand},
							.active = widget->twoPcSets.at(widget->set),
							.onToggle = [this](bool active) {
								setState([&]() {
									widget->twoPcSets.at(widget->set) = active;
								});
							},
							.child = "2pc",
						},
						ToggleButton{
							.widget{.width = Size::Expand},
							.active = widget->fourPcSets.at(widget->set),
							.onToggle = [this](bool active) {
								setState([&]() {
									widget->fourPcSets.at(widget->set) = active;
								});
							},
							.child = "4pc",
						},
					},
				};

				return UI::Card{
					.child = Column{
						.widget{
							.padding = 4.f,
						},
						.spacing = 4.f,
						.children{
							header,
							footer,
							opts,
						},
					},
				};
			}
		};
	};
}// namespace

squi::core::Child UI::OptimizationSetChooser::State::build(const Element &element) {
	auto setSet = [this](std::unordered_map<Artifact::SetKey, bool> &set, bool value) {
		setState([&]() {
			for (auto &[key, active]: set) {
				active = value;
			}
		});
	};

	auto helperButtons = Grid{
		.columnCount = Grid::MinSize{250.f},
		.spacing = 4.f,
		.children{
			Card{
				.widget{.padding = 4.f},
				.child = Column{
					.spacing = 4.f,
					.children{
						Button{
							.widget{.width = Size::Expand},
							.onClick = [&, this]() {
								setSet(widget->twoPcSets, true);
							},
							.child = "Enable all 2pc sets",
						},
						Button{
							.widget{.width = Size::Expand},
							.theme = Button::Theme::Standard(),
							.onClick = [&, this]() {
								setSet(widget->twoPcSets, false);
							},
							.child = "Disable all 2pc sets",
						},
					},
				},
			},
			Card{
				.widget{.padding = 4.f},
				.child = Column{
					.spacing = 4.f,
					.children{
						Button{
							.widget{.width = Size::Expand},
							.onClick = [&, this]() {
								setSet(widget->fourPcSets, true);
							},
							.child = "Enable all 4pc sets",
						},
						Button{
							.widget{.width = Size::Expand},
							.theme = Button::Theme::Standard(),
							.onClick = [&, this]() {
								setSet(widget->fourPcSets, false);
							},
							.child = "Disable all 4pc sets",
						},
					},
				},
			},
		},
	};

	return Dialog{
		.width = 1000.f,
		.closeEvent = closeEvent,
		.title = "Choose enabled sets",
		.content = Column{
			.spacing = 4.f,
			.children{
				helperButtons,
				Grid{
					.columnCount = Grid::MinSize{250.f},
					.spacing = 4.f,
					.children = [&]() {
						Children ret;

						for (const auto &[key, set]: Artifact::sets) {
							ret.emplace_back(OptimizationSetChooserItem{
								.set = key,
								.character = widget->character,
								.ctx = widget->ctx,
								.twoPcSets = widget->twoPcSets,
								.fourPcSets = widget->fourPcSets,
							});
						}

						return ret;
					}(),
				},
			},
		},
		.buttons{
			Container{},
			Button{
				.widget{
					.width = Size::Expand,
				},
				.theme = Button::Theme::Standard(),
				.onClick = [this]() {
					closeEvent.notify();
				},
				.child = "Close",
			},
		},
	};
}
