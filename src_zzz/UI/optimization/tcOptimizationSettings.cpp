#include "tcOptimizationSettings.hpp"

#include "UI/engine/engineCard.hpp"
#include "modifiers/disc/subStats.hpp"
#include "ranges"
#include "store.hpp"
#include "widgets/column.hpp"
#include "widgets/dropdownButton.hpp"
#include "widgets/iconButton.hpp"
#include "widgets/numberBox.hpp"
#include "widgets/row.hpp"
#include "widgets/slider.hpp"
#include "widgets/text.hpp"


using namespace squi;
squi::core::Child UI::TCOptimizationSettings::State::build(const Element &element) {
	Child engineCard = EngineCard{
		.widget{
			.width = Size::Expand,
			.height = Size::Shrink,
		},
		.engine = widget->agent.state.loadout().engineInstanceKey ? &Store::engines.at(widget->agent.state.loadout().engineInstanceKey) : nullptr,
		.agentKey = widget->agent.instanceKey,
		.actions = EngineCard::Actions::agent,
	};

	Child mainStats = Column{
		.spacing = 4.f,
		.children = [&]() {
			Children ret;

			for (const auto &[index, partition]: Disc::partitions | std::views::enumerate) {
				Observable<const std::string &> mainStatTextUpdater;
				Observable<const std::string &> rarityTextUpdater;

				ret.emplace_back(Row{
					.widget{
						.height = Size::Shrink,
					},
					.spacing = 4.f,
					.children{
						DropdownButton{
							.widget{
								.width = Size::Expand,
							},
							.theme = Button::Theme::Standard(),
							.text = Utils::Stringify(widget->agent.state.loadout().disc.getTheorycraft().mainStats.at(index).stat),
							.items = [&]() {
								std::vector<ContextMenu::Item> ret;

								for (const auto &mainStat: Stats::Disc::byPartition(partition)) {
									ret.emplace_back(ContextMenu::Button{
										.text = Utils::Stringify(mainStat),
										.callback = [this, index, mainStat]() {
											auto &loadout = widget->agent.state.loadout();
											if (!loadout.disc.isTheorycraft()) return;
											auto &discs = std::get<Stats::Disc::Theorycraft>(loadout.disc.equipped);
											setState([&]() {
												discs.mainStats.at(index).stat = mainStat;
											});
											discs.updateStats();
											loadout.disc.refreshStats();
											widget->agent.updateEvent.notify();
										},
									});
								}

								return ret;
							}(),
						},
						DropdownButton{
							.widget{
								.width = Size::Wrap,
							},
							.theme = Button::Theme::Standard(),
							.text = std::format("{} star", widget->agent.state.loadout().disc.getTheorycraft().mainStats.at(index).rarity),
							.items = [&]() {
								std::vector<ContextMenu::Item> ret;

								for (uint32_t i = 1; i <= 5; i++) {
									ret.emplace_back(ContextMenu::Button{
										.text = std::format("{} star", i),
										.callback = [this, index, i]() {
											auto &loadout = widget->agent.state.loadout();
											if (!loadout.disc.isTheorycraft()) return;
											auto &discs = std::get<Stats::Disc::Theorycraft>(loadout.disc.equipped);
											setState([&]() {
												discs.mainStats.at(index).rarity = i;
											});
											discs.updateStats();
											loadout.disc.refreshStats();
											widget->agent.updateEvent.notify();
										},
									});
								}

								return ret;
							}(),
						},
						NumberBox{
							.value = static_cast<float>(widget->agent.state.loadout().disc.getTheorycraft().mainStats.at(index).level),
							.min = 0.f,
							.max = 20.f,
							.precision = 0,
							.onChange = [this, index](float newVal) {
								if (!widget->agent.state.loadout().disc.isTheorycraft()) return;
								setState([&]() {
									widget->agent.state.loadout().disc.getTheorycraft().mainStats.at(index).level = std::floor(newVal);
									widget->agent.state.loadout().disc.getTheorycraft().updateStats();
									widget->agent.state.loadout().disc.refreshStats();
								});
								widget->agent.updateEvent.notify();
							},
						},
					},
				});
			}

			static constexpr auto setKeyToName = [](Disc::SetKey key) -> std::string {
				if (!key) return "No set";
				return std::string{Disc::sets.at(key).name};
			};


			Observable<const std::string &> set1TextUpdater;
			Observable<const std::string &> set2TextUpdater;
			Observable<const std::string &> set1TypeTextUpdater;
			ret.emplace_back(Row{
				.widget{
					.height = Size::Shrink,
				},
				.spacing = 4.f,
				.children{
					DropdownButton{
						.widget{
							.width = Size::Expand,
						},
						.theme = Button::Theme::Standard(),
						.text = setKeyToName(widget->agent.state.loadout().disc.getTheorycraft().set1.key),
						.items = [&]() {
							std::vector<ContextMenu::Item> ret;

							for (const auto &[key, set]: Disc::sets) {
								ret.emplace_back(ContextMenu::Button{
									.text = setKeyToName(key),
									.callback = [this, key]() {
										if (!widget->agent.state.loadout().disc.isTheorycraft()) return;
										setState([&]() {
											widget->agent.state.loadout().disc.getTheorycraft().set1.key = key;
											widget->agent.state.loadout().disc.refreshStats();
										});
										widget->agent.updateEvent.notify();
									},
								});
							}

							return ret;
						}(),
					},
					DropdownButton{
						.widget{
							.width = Size::Shrink,
							.height = Size::Shrink,
						},
						.theme = Button::Theme::Standard(),
						.text = Utils::Stringify(widget->agent.state.loadout().disc.getTheorycraft().set1.type),
						.items = [&]() {
							std::vector<ContextMenu::Item> ret;

							for (const auto &type: {Stats::Disc::Theorycraft::Set::Type::twoPc, Stats::Disc::Theorycraft::Set::Type::fourPc}) {
								ret.emplace_back(ContextMenu::Button{
									.text = Utils::Stringify(type),
									.callback = [this, type]() {
										if (!widget->agent.state.loadout().disc.isTheorycraft()) return;
										setState([&]() {
											widget->agent.state.loadout().disc.getTheorycraft().set1.type = type;
											widget->agent.state.loadout().disc.refreshStats();
										});
										widget->agent.updateEvent.notify();
									},
								});
							}

							return ret;
						}(),
					},
					IconButton{
						.icon = 0xe5cd,
						.theme = Button::Theme::Standard(),
						.onClick = [this]() {
							if (!widget->agent.state.loadout().disc.isTheorycraft()) return;
							setState([&]() {
								widget->agent.state.loadout().disc.getTheorycraft().set1.key.clear();
								widget->agent.state.loadout().disc.refreshStats();
							});
							widget->agent.updateEvent.notify();
						},
					},
				},
			});

			ret.emplace_back(Row{
				.widget{
					.height = Size::Shrink,
				},
				.spacing = 4.f,
				.children = {
					DropdownButton{
						.widget{
							.width = Size::Expand,
						},
						.theme = Button::Theme::Standard(),
						.disabled = !(widget->agent.state.loadout().disc.getTheorycraft().set1.key && widget->agent.state.loadout().disc.getTheorycraft().set1.type == Stats::Disc::Theorycraft::Set::Type::twoPc),
						.text = setKeyToName(widget->agent.state.loadout().disc.getTheorycraft().set2.key),
						.items = [&]() {
							std::vector<ContextMenu::Item> ret;

							for (const auto &[key, set]: Disc::sets) {
								ret.emplace_back(ContextMenu::Button{
									.text = setKeyToName(key),
									.callback = [this, key]() {
										if (!widget->agent.state.loadout().disc.isTheorycraft()) return;
										setState([&]() {
											widget->agent.state.loadout().disc.getTheorycraft().set2 = key;
											widget->agent.state.loadout().disc.refreshStats();
										});
										widget->agent.updateEvent.notify();
									},
								});
							}

							return ret;
						}(),
					},
					IconButton{
						.icon = 0xe5cd,
						.theme = Button::Theme::Standard(),
						.onClick = [this]() {
							if (!widget->agent.state.loadout().disc.isTheorycraft()) return;
							setState([&]() {
								widget->agent.state.loadout().disc.getTheorycraft().set2.clear();
								widget->agent.state.loadout().disc.refreshStats();
							});
							widget->agent.updateEvent.notify();
						},
					},
				},
			});

			return ret;
		}(),
	};

	Child subStats = Column{
		.children = [&]() {
			Children ret;

			for (const auto &subStat: Stats::subStats) {
				ret.emplace_back(Column{
					.children{
						Row{
							.spacing = 4.f,
							.children{
								Box{
									.widget{
										.width = Size::Shrink,
										.padding = 4.f,
									},
									.color = Color::css(0x0, 0.1f),
									.borderColor = Color::css(0x0, 0.2f),
									.borderWidth = 1.f,
									.borderRadius = 4.f,
									.borderPosition = squi::Box::BorderPosition::outset,
									.child = Text{
										.widget{
											.alignment = Alignment::Center,
										},
										.text = Formula::Percentage({}, Modifiers::Disc::subStats().fromStat(subStat).eval(widget->ctx), Utils::isPercentage(subStat)),
									},
								},
								Box{
									.widget{
										.padding = 4.f,
									},
									.color = Color::css(0x0, 0.1f),
									.borderColor = Color::css(0x0, 0.2f),
									.borderWidth = 1.f,
									.borderRadius = 4.f,
									.borderPosition = squi::Box::BorderPosition::outset,
									.child = Text{
										.widget{
											.alignment = Alignment::Center,
										},
										.text = Utils::Stringify(subStat),
									},
								},
								Box{
									.widget{
										.width = Size::Shrink,
										.padding = 4.f,
									},
									.color = Color::css(0x0, 0.1f),
									.borderColor = Color::css(0x0, 0.2f),
									.borderWidth = 1.f,
									.borderRadius = 4.f,
									.borderPosition = squi::Box::BorderPosition::outset,
									.child = Text{
										.widget{
											.alignment = Alignment::Center,
										},
										.text = std::format("{} rolls", widget->agent.state.loadout().disc.getTheorycraft().fromStat(subStat)),
									},
								},
							},
						},
						Slider{
							.minValue = 0.f,
							.maxValue = 24.f,
							.value = static_cast<float>(widget->agent.state.loadout().disc.getTheorycraft().fromStat(subStat)),
							.ticks = Slider::TickInterval{1.f},
							.onChange = [this, subStat](float newVal) {
								auto val = static_cast<uint32_t>(std::round(newVal));
								if (!widget->agent.state.loadout().disc.isTheorycraft()) return;
								auto &loadout = widget->agent.state.loadout();
								auto &theorycraft = std::get<Stats::Disc::Theorycraft>(loadout.disc.equipped);
								if (val != theorycraft.fromStat(subStat)) {
									setState([&]() {
										theorycraft.fromStat(subStat) = static_cast<uint8_t>(val);
										theorycraft.updateStats();
										loadout.disc.refreshStats();
									});
									widget->agent.updateEvent.notify();
								}
							},
						},
					},
				});
			}

			return ret;
		}(),
	};
	return Row{
		.spacing = 4.f,
		.children{
			engineCard,
			mainStats,
			subStats,
		},
	};
}
