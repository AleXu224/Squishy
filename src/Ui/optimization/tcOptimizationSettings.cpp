#include "tcOptimizationSettings.hpp"

#include "Ui/weapon/weaponCard.hpp"
#include "align.hpp"
#include "column.hpp"
#include "dropdownButton.hpp"
#include "modifiers/artifact/subStats.hpp"
#include "numberBox.hpp"
#include "ranges"
#include "row.hpp"
#include "store.hpp"
#include "text.hpp"
#include "widgets/iconButton.hpp"
#include "widgets/slider.hpp"


using namespace squi;

UI::TCOptimizationSettings::operator squi::Child() const {
	auto &weapon = Store::weapons.at(character.state.loadout().weaponInstanceKey);

	auto weaponCard = WeaponCard{
		.widget{
			.width = Size::Expand,
			.height = Size::Shrink,
		},
		.weapon = weapon,
		.actions = WeaponCard::Actions::character,
	};

	auto mainStats = Column{
		.spacing = 4.f,
		.children = [&]() {
			Children ret;

			for (const auto &[index, slot]: Artifact::slots | std::views::enumerate) {
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
								.onInit = [&character = character, index, mainStatTextUpdater](Widget &w) {
									observe(w, character.updateEvent, [index, mainStatTextUpdater, &character]() {
										if (!character.state.loadout().artifact.isTheorycraft()) return;
										mainStatTextUpdater.notify(Utils::Stringify(character.state.loadout().artifact.getTheorycraft().mainStats.at(index).stat));
									});
								},
							},
							.style = ButtonStyle::Standard(),
							.text = Utils::Stringify(character.state.loadout().artifact.getTheorycraft().mainStats.at(index).stat),
							.items = [&]() {
								std::vector<ContextMenu::Item> ret;

								for (const auto &mainStat: Stats::Artifact::bySlot(slot)) {
									ret.emplace_back(ContextMenu::Item{
										.text = Utils::Stringify(mainStat),
										.content = [&character = character, index, mainStat]() {
											auto &loadout = character.state.loadout();
											if (!loadout.artifact.isTheorycraft()) return;
											auto &artifacts = std::get<Stats::Artifact::Theorycraft>(loadout.artifact.equipped);
											artifacts.mainStats.at(index).stat = mainStat;
											artifacts.updateStats();
											loadout.artifact.refreshStats();
											character.updateEvent.notify();
										},
									});
								}

								return ret;
							}(),
							.textUpdater = mainStatTextUpdater,
						},
						DropdownButton{
							.widget{
								.width = Size::Shrink,
								.onInit = [&character = character, index, rarityTextUpdater](Widget &w) {
									observe(w, character.updateEvent, [index, rarityTextUpdater, &character]() {
										if (!character.state.loadout().artifact.isTheorycraft()) return;
										rarityTextUpdater.notify(std::format("{} star", character.state.loadout().artifact.getTheorycraft().mainStats.at(index).rarity));
									});
								},
							},
							.style = ButtonStyle::Standard(),
							.text = std::format("{} star", character.state.loadout().artifact.getTheorycraft().mainStats.at(index).rarity),
							.items = [&]() {
								std::vector<ContextMenu::Item> ret;

								for (uint32_t i = 1; i <= 5; i++) {
									ret.emplace_back(ContextMenu::Item{
										.text = std::format("{} star", i),
										.content = [&character = character, index, i]() {
											auto &loadout = character.state.loadout();
											if (!loadout.artifact.isTheorycraft()) return;
											auto &artifacts = std::get<Stats::Artifact::Theorycraft>(loadout.artifact.equipped);
											artifacts.mainStats.at(index).rarity = i;
											artifacts.updateStats();
											loadout.artifact.refreshStats();
											character.updateEvent.notify();
										},
									});
								}

								return ret;
							}(),
							.textUpdater = rarityTextUpdater,
						},
						NumberBox{
							.value = static_cast<float>(character.state.loadout().artifact.getTheorycraft().mainStats.at(index).level),
							.min = 0.f,
							.max = 20.f,
							.onChange = [index, &character = character](float newVal) {
								if (!character.state.loadout().artifact.isTheorycraft()) return;
								character.state.loadout().artifact.getTheorycraft().mainStats.at(index).level = std::floor(newVal);
								character.state.loadout().artifact.getTheorycraft().updateStats();
								character.state.loadout().artifact.refreshStats();
								character.updateEvent.notify();
							},
							.validator = [](float val) {
								return TextBox::Controller::ValidatorResponse{
									.valid = (val == std::round(val)),
									.message = "Value must be an intereger",
								};
							},
						},
					},
				});
			}

			static constexpr auto setKeyToName = [](Artifact::SetKey key) -> std::string {
				if (!key) return "No set";
				return std::string{Artifact::sets.at(key).name};
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
							.onInit = [&character = character, set1TextUpdater](Widget &w) {
								observe(w, character.updateEvent, [set1TextUpdater, &character]() {
									if (!character.state.loadout().artifact.isTheorycraft()) return;
									set1TextUpdater.notify(setKeyToName(character.state.loadout().artifact.getTheorycraft().set1.key));
								});
							},
						},
						.style = ButtonStyle::Standard(),
						.text = setKeyToName(character.state.loadout().artifact.getTheorycraft().set1.key),
						.items = [&character = character]() {
							std::vector<ContextMenu::Item> ret;

							for (const auto &[key, set]: Artifact::sets) {
								ret.emplace_back(ContextMenu::Item{
									.text = setKeyToName(key),
									.content = [key, &character]() {
										if (!character.state.loadout().artifact.isTheorycraft()) return;
										character.state.loadout().artifact.getTheorycraft().set1.key = key;
										character.state.loadout().artifact.refreshStats();
										character.updateEvent.notify();
									},
								});
							}

							return ret;
						}(),
						.textUpdater = set1TextUpdater,
					},
					DropdownButton{
						.widget{
							.width = Size::Shrink,
							.height = Size::Shrink,
							.onInit = [&character = character, set1TypeTextUpdater](Widget &w) {
								observe(w, character.updateEvent, [set1TypeTextUpdater, &character]() {
									if (!character.state.loadout().artifact.isTheorycraft()) return;
									set1TypeTextUpdater.notify(Utils::Stringify(character.state.loadout().artifact.getTheorycraft().set1.type));
								});
							},
						},
						.style = ButtonStyle::Standard(),
						.text = Utils::Stringify(character.state.loadout().artifact.getTheorycraft().set1.type),
						.items = [&character = character]() {
							std::vector<ContextMenu::Item> ret;

							for (const auto &type: {Stats::Artifact::Theorycraft::Set::Type::twoPc, Stats::Artifact::Theorycraft::Set::Type::fourPc}) {
								ret.emplace_back(ContextMenu::Item{
									.text = Utils::Stringify(type),
									.content = [&character, type]() {
										if (!character.state.loadout().artifact.isTheorycraft()) return;
										character.state.loadout().artifact.getTheorycraft().set1.type = type;
										character.state.loadout().artifact.refreshStats();
										character.updateEvent.notify();
									},
								});
							}

							return ret;
						}(),
						.textUpdater = set1TypeTextUpdater,
					},
					IconButton{
						.icon = 0xe5cd,
						.style = ButtonStyle::Standard(),
						.onClick = [&character = character](GestureDetector::Event event) {
							if (!character.state.loadout().artifact.isTheorycraft()) return;
							character.state.loadout().artifact.getTheorycraft().set1.key.clear();
							character.state.loadout().artifact.refreshStats();
							character.updateEvent.notify();
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
							.onInit = [&character = character, set2TextUpdater](Widget &w) {
								observe(w, character.updateEvent, [set2TextUpdater, &w, &character]() {
									if (!character.state.loadout().artifact.isTheorycraft()) return;
									auto &theorycraft = character.state.loadout().artifact.getTheorycraft();
									set2TextUpdater.notify(setKeyToName(theorycraft.set2.key));
									Button::State::disabled.of(w) = !(theorycraft.set1.key && theorycraft.set1.type == Stats::Artifact::Theorycraft::Set::Type::twoPc);
								});
							},
						},
						.style = ButtonStyle::Standard(),
						.text = setKeyToName(character.state.loadout().artifact.getTheorycraft().set2.key),
						.disabled = !(character.state.loadout().artifact.getTheorycraft().set1.key && character.state.loadout().artifact.getTheorycraft().set1.type == Stats::Artifact::Theorycraft::Set::Type::twoPc),
						.items = [&character = character]() {
							std::vector<ContextMenu::Item> ret;

							for (const auto &[key, set]: Artifact::sets) {
								ret.emplace_back(ContextMenu::Item{
									.text = setKeyToName(key),
									.content = [key, &character]() {
										if (!character.state.loadout().artifact.isTheorycraft()) return;
										character.state.loadout().artifact.getTheorycraft().set2 = key;
										character.state.loadout().artifact.refreshStats();
										character.updateEvent.notify();
									},
								});
							}

							return ret;
						}(),
						.textUpdater = set2TextUpdater,
					},
					IconButton{
						.icon = 0xe5cd,
						.style = ButtonStyle::Standard(),
						.onClick = [&character = character](GestureDetector::Event event) {
							if (!character.state.loadout().artifact.isTheorycraft()) return;
							character.state.loadout().artifact.getTheorycraft().set2.clear();
							character.state.loadout().artifact.refreshStats();
							character.updateEvent.notify();
						},
					},
				},
			});

			return ret;
		}(),
	};

	auto subStats = Column{
		.children = [&character = character, &ctx = ctx]() {
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
									.child = Align{
										.child = Text{
											.widget{
												.onInit = [&character, subStat, ctx](Widget &w) {
													observe(w, character.updateEvent, [&w, subStat, ctx]() {
														auto &text = w.as<Text::Impl>();
														text.setText(Formula::Percentage({}, Modifiers::Artifact::subStats().fromStat(subStat).eval(ctx), Utils::isPercentage(subStat)));
													});
												},
											},
											.text = Formula::Percentage({}, Modifiers::Artifact::subStats().fromStat(subStat).eval(ctx), Utils::isPercentage(subStat)),
										},
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
									.child = Align{
										.child = Text{
											.text = Utils::Stringify(subStat),
										},
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
									.child = Align{
										.child = Text{
											.widget{
												.onInit = [&character, subStat](Widget &w) {
													observe(w, character.updateEvent, [&w, subStat, &character]() {
														if (!character.state.loadout().artifact.isTheorycraft()) return;
														auto &text = w.as<Text::Impl>();
														text.setText(std::format("{} rolls", character.state.loadout().artifact.getTheorycraft().fromStat(subStat)));
													});
												},
											},
											.text = std::format("{} rolls", character.state.loadout().artifact.getTheorycraft().fromStat(subStat)),
										},
									},
								},
							},
						},
						Slider{
							.value = static_cast<float>(character.state.loadout().artifact.getTheorycraft().fromStat(subStat)),
							.minVal = 0.f,
							.maxVal = 24.f,
							.step = 1.f,
							.valueChanged = [subStat, &character](float newVal) {
								if (!character.state.loadout().artifact.isTheorycraft()) return;
								auto &loadout = character.state.loadout();
								auto &theorycraft = std::get<Stats::Artifact::Theorycraft>(loadout.artifact.equipped);
								theorycraft.fromStat(subStat) = static_cast<uint8_t>(newVal);
								theorycraft.updateStats();
								loadout.artifact.refreshStats();
								character.updateEvent.notify();
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
			weaponCard,
			mainStats,
			subStats,
		},
	};
}
