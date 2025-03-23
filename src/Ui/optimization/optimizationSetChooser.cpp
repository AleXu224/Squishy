#include "optimizationSetChooser.hpp"
#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/grid.hpp"
#include "button.hpp"
#include "column.hpp"
#include "container.hpp"
#include "dialog.hpp"

#include "Ui/utils/card.hpp"
#include "artifact/key.hpp"
#include "artifact/sets.hpp"
#include "image.hpp"
#include "rebuilder.hpp"
#include "row.hpp"
#include "text.hpp"
#include "widgets/toggleButton.hpp"


using namespace squi;

namespace {
	struct OptimizationSetChooserItem {
		// Args
		squi::Widget::Args widget{};
		Artifact::SetKey set;
		Character::Instance &character;
		const Formula::Context &ctx;
		std::unordered_map<Artifact::SetKey, bool> &twoPcSets;
		std::unordered_map<Artifact::SetKey, bool> &fourPcSets;

		operator squi::Child() const {
			const auto &data = Artifact::sets.at(set);

			auto header = Row{
				.widget{.height = Size::Shrink},
				.alignment = squi::Row::Alignment::center,
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
								Artifact::sets.at(set).name,
								Utils::Stringify(Artifact::Slot::flower)
							)
						)
					},
					Container{
						.widget{.height = Size::Shrink},
						.child = Text{
							.text = data.name,
							.lineWrap = true,
						},
					},
				},
			};

			auto opts = Column{
				.widget{
					.height = Size::Shrink,
				},
				.spacing = 4.f,
				.children = [&]() {
					Children ret;

					auto instanceKey = character.instanceKey;

					auto opt_lists = std::vector{
						std::ref(data.data.twoPc.opts),
						std::ref(data.data.fourPc.opts),
					};

					for (auto &opt_list: opt_lists) {
						for (auto &opt: opt_list.get()) {
							std::visit(
								Utils::overloaded{
									[&](Option::Boolean &opt) {
										if (opt.displayCondition && !opt.displayCondition.value().eval(ctx)) return;
										ret.emplace_back(UI::ToggleOption{
											.option = opt,
											.instanceKey = instanceKey,
											.ctx = ctx,
										});
									},
									[&](Option::ValueList &opt) {
										if (opt.displayCondition && !opt.displayCondition.value().eval(ctx)) return;
										ret.emplace_back(UI::ValueListOption{
											.option = opt,
											.instanceKey = instanceKey,
											.ctx = ctx,
										});
									},
								},
								character.loadout.artifact.options.at(
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

			auto footer = Row{
				.widget{
					.height = Size::Shrink,
				},
				.children{
					ToggleButton{
						.widget{.width = Size::Expand},
						.text = "2pc",
						.active = twoPcSets.at(set),
						.modifyStyle = [](ButtonStyle &style, bool) {
							style.borderRadius = style.borderRadius.withRight(0.f);
							style.borderWidth = style.borderWidth.withRight(0.f);
						},
						.onSwitch = [set = set, &twoPcSets = twoPcSets](bool active) {
							twoPcSets.at(set) = active;
						},
					},
					ToggleButton{
						.widget{.width = Size::Expand},
						.text = "4pc",
						.active = fourPcSets.at(set),
						.modifyStyle = [](ButtonStyle &style, bool) {
							style.borderRadius = style.borderRadius.withLeft(0.f);
							style.borderWidth = style.borderWidth.withLeft(0.f);
						},
						.onSwitch = [set = set, &fourPcSets = fourPcSets](bool active) {
							fourPcSets.at(set) = active;
						},
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
}// namespace

UI::OptimizationSetChooser::operator squi::Child() const {
	VoidObservable closeEvent;

	auto setSet = [&, updateEvent = character.updateEvent](std::unordered_map<Artifact::SetKey, bool> &set, bool value) {
		for (auto &[key, active]: set) {
			active = value;
		}
		updateEvent.notify();
	};

	auto helperButtons = Grid{
		.spacing = 4.f,
		.columnCount = Grid::MinSize{250.f},
		.children{
			Card{
				.widget{.padding = 4.f},
				.child = Column{
					.spacing = 4.f,
					.children{
						Button{
							.widget{.width = Size::Expand},
							.text = "Enable all 2pc sets",
							.onClick = [setSet, &twoPcSets = twoPcSets](GestureDetector::Event) {
								setSet(twoPcSets, true);
							},
						},
						Button{
							.widget{.width = Size::Expand},
							.text = "Disable all 2pc sets",
							.style = ButtonStyle::Standard(),
							.onClick = [setSet, &twoPcSets = twoPcSets](GestureDetector::Event) {
								setSet(twoPcSets, false);
							},
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
							.text = "Enable all 4pc sets",
							.onClick = [setSet, &fourPcSets = fourPcSets](GestureDetector::Event) {
								setSet(fourPcSets, true);
							},
						},
						Button{
							.widget{.width = Size::Expand},
							.text = "Disable all 4pc sets",
							.style = ButtonStyle::Standard(),
							.onClick = [setSet, &fourPcSets = fourPcSets](GestureDetector::Event) {
								setSet(fourPcSets, false);
							},
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
				Rebuilder{
					.rebuildEvent = character.updateEvent,
					.buildFunc = [&twoPcSets = twoPcSets, &fourPcSets = fourPcSets, &character = character, ctx = ctx]() -> Child {
						return Grid{
							.spacing = 4.f,
							.columnCount = Grid::MinSize{250.f},
							.children = [&]() {
								Children ret;

								for (const auto &[key, set]: Artifact::sets) {
									ret.emplace_back(OptimizationSetChooserItem{
										.set = key,
										.character = character,
										.ctx = ctx,
										.twoPcSets = twoPcSets,
										.fourPcSets = fourPcSets,
									});
								}

								return ret;
							}(),
						};
					},
				},
			},
		},
		.buttons{
			Container{},
			Button{
				.widget{
					.width = Size::Expand,
				},
				.text = "Close",
				.style = ButtonStyle::Standard(),
				.onClick = [closeEvent](GestureDetector::Event) {
					closeEvent.notify();
				},
			},
		},
	};
}
