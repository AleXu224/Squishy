#include "artifactEditor.hpp"

#include "artifact/sets.hpp"

#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "container.hpp"
#include "dropdownButton.hpp"
#include "gestureDetector.hpp"
#include "numberBox.hpp"
#include "observer.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "text.hpp"

using namespace squi;

struct ArtifactEditorItem {
	// Args
	squi::Widget::Args widget{};
	std::string_view name;
	Child child;

	operator squi::Child() const {
		return Stack{
			.widget = widget.withDefaultHeight(Size::Shrink),
			.children{
				Container{
					.widget{
						.width = Size::Shrink,
						.height = 32.f,
					},
					.child = Align{
						.xAlign = 0.f,
						.child = Text{.text = name},
					},
				},
				Align{
					.xAlign = 1.f,
					.yAlign = 0.f,
					.child = child,
				},
			},
		};
	}
};
struct ArtifactEditorSubstat {
	// Args
	squi::Widget::Args widget{};
	std::string_view name;
	Children children;

	operator squi::Child() const {
		return Stack{
			.widget = widget.withDefaultHeight(Size::Shrink),
			.children{
				Container{
					.widget{
						.width = Size::Shrink,
						.height = 32.f,
					},
					.child = Align{
						.xAlign = 0.f,
						.child = Text{.text = name},
					},
				},
				Align{
					.xAlign = 1.f,
					.yAlign = 0.f,
					.child = Row{
						.widget{
							.width = Size::Shrink,
							.height = Size::Shrink,
						},
						.spacing = 4.f,
						.children = children,
					},
				},
			},
		};
	}
};

UI::ArtifactEditor::operator squi::Child() const {
	auto storage = std::make_shared<Storage>(artifact.value_or(Artifact::Instance{}));

	VoidObservable closeEvent{};
	Observable<const Artifact::Set &> setChangeEvent{};
	Observable<Artifact::Slot> slotChangeEvent{};
	Observable<Stat> mainStatChangeEvent{};

	// Level
	Child levelSelector = ArtifactEditorItem{
		.name = "Level",
		.child = NumberBox{
			.value = static_cast<float>(storage->artifact.level),
			.min = 0.f,
			.max = 20.f,
			.onChange = [storage](float newVal) {
				std::println("{}", newVal);
				storage->artifact.level = std::floor(newVal);
			},
			.validator = [](float val) {
				return TextBox::Controller::ValidatorResponse{
					.valid = (val == std::round(val)),
					.message = "Value must be an intereger",
				};
			},
		},
	};

	// Set
	decltype(DropdownButton::textUpdater) setTextUpdater{};
	auto setItemsProvider = [setChangeEvent]() {
		std::vector<ContextMenu::Item> ret{};
		ret.reserve(Artifact::sets.size());
		for (const auto &set: Artifact::sets) {
			ret.emplace_back(ContextMenu::Item{
				.text = std::string(set.second.name),
				.content = [set = set.second, setChangeEvent]() {
					setChangeEvent.notify(set);
				},
			});
		}
		return ret;
	};
	Child setSelector = ArtifactEditorItem{
		.name = "Set",
		.child = DropdownButton{
			.widget{
				.onInit = [setChangeEvent, setTextUpdater, setItemsProvider](Widget &w) {
					w.customState.add(setChangeEvent.observe([setTextUpdater, setItemsProvider](const Artifact::Set &set) {
						setTextUpdater.notify(std::string{set.name});
					}));
				},
			},
			.style = ButtonStyle::Standard(),
			.text = storage->artifact.set.key != 0 ? Artifact::sets.at(storage->artifact.set).name : "No set",
			.items = setItemsProvider(),
			.textUpdater = setTextUpdater,
		},
	};

	// Slot
	decltype(DropdownButton::textUpdater) slotTextUpdater{};
	Child slotSelector = ArtifactEditorItem{
		.name = "Slot",
		.child = DropdownButton{
			.widget{
				.onInit = [slotChangeEvent, slotTextUpdater](Widget &w) {
					w.customState.add(slotChangeEvent.observe([slotTextUpdater](Artifact::Slot newSlot) {
						slotTextUpdater.notify(Utils::Stringify(newSlot));
					}));
				},
			},
			.style = ButtonStyle::Standard(),
			.text = Utils::Stringify(storage->artifact.slot),
			.items = [&]() {
				std::vector<ContextMenu::Item> ret{};
				ret.reserve(Artifact::slots.size());
				for (const auto &slot: Artifact::slots) {
					ret.emplace_back(ContextMenu::Item{
						.text = Utils::Stringify(slot),
						.content = [slot, slotChangeEvent]() {
							slotChangeEvent.notify(slot);
						},
					});
				}
				return ret;
			}(),
			.textUpdater = slotTextUpdater,
		},
	};

	// Main stat
	decltype(DropdownButton::textUpdater) mainStatTextUpdater{};
	decltype(DropdownButton::itemsUpdater) mainStatItemsUpdater{};
	auto mainStatItemsProvider = [storage, mainStatChangeEvent](Artifact::Slot slot) {
		std::vector<ContextMenu::Item> ret{};
		for (const auto &stat: Stats::Artifact::bySlot(slot)) {
			ret.emplace_back(ContextMenu::Item{
				.text = Utils::Stringify(stat),
				.content = [stat, mainStatChangeEvent]() {
					mainStatChangeEvent.notify(stat);
				},
			});
		}
		return ret;
	};
	Child mainStatSelector = ArtifactEditorItem{
		.name = "Main Stat",
		.child = DropdownButton{
			.widget{
				.onInit = [slotChangeEvent, mainStatItemsUpdater, mainStatItemsProvider, storage, mainStatChangeEvent, mainStatTextUpdater](Widget &w) {
					w.customState.add(slotChangeEvent.observe([mainStatItemsUpdater, mainStatItemsProvider, storage, mainStatChangeEvent](Artifact::Slot newSlot) {
						mainStatItemsUpdater.notify(mainStatItemsProvider(newSlot));
						auto &artifact = storage->artifact;
						const auto &stats = Stats::Artifact::bySlot(newSlot);
						if (auto it = std::ranges::find(stats, artifact.mainStat); it == stats.end()) {
							mainStatChangeEvent.notify(stats.at(0));
						}
					}));
					w.customState.add(mainStatChangeEvent.observe([mainStatTextUpdater, storage](Stat newStat) {
						storage->artifact.mainStat = newStat;
						mainStatTextUpdater.notify(Utils::Stringify(newStat));
					}));
				},
			},
			.style = ButtonStyle::Standard(),
			.text = Utils::Stringify(storage->artifact.mainStat),
			.items = mainStatItemsProvider(storage->artifact.slot),
			.textUpdater = mainStatTextUpdater,
			.itemsUpdater = mainStatItemsUpdater,
		},
	};

	// Substats
	auto createSubStat = [storage](size_t subStatIndex) {
		decltype(TextBox::Controller::disable) subStatDisable{};
		decltype(DropdownButton::textUpdater) subStatTextUpdater{};
		decltype(NumberBox::valueUpdater) subStatValueUpdater{};
		Observable<std::optional<Stat>> subStatChangeEvent{};
		auto subStatItemsProvider = [storage, subStatChangeEvent]() {
			std::vector<ContextMenu::Item> ret{};
			ret.emplace_back(ContextMenu::Item{
				.text = "None",
				.content = [subStatChangeEvent]() {
					subStatChangeEvent.notify({});
				},
			});
			for (const auto &stat: std::views::filter(Stats::Artifact::subStats, [&](auto &&val) {
					 return storage->artifact.mainStat != val;
				 })) {
				ret.emplace_back(ContextMenu::Item{
					.text = Utils::Stringify(stat),
					.content = [stat, subStatChangeEvent]() {
						subStatChangeEvent.notify(stat);
					},
				});
			}
			return ret;
		};
		Child subStat = ArtifactEditorSubstat{
			.name = std::format("Substat {}", subStatIndex + 1),
			.children{
				DropdownButton{
					.widget{
						.onInit = [subStatChangeEvent, subStatTextUpdater, subStatDisable, storage, subStatIndex, subStatValueUpdater](Widget &w) {
							w.customState.add(subStatChangeEvent.observe([subStatTextUpdater, subStatDisable, storage, subStatIndex, subStatValueUpdater](std::optional<Stat> newStat) {
								auto prevStat = storage->artifact.subStats.at(subStatIndex).and_then([](const StatValue &val) -> std::optional<Stat> {
									return val.stat;
								});
								storage->artifact.subStats.at(subStatIndex) = newStat.transform([storage, subStatIndex](auto &&val) {
									return StatValue{
										.stat = val,
										.value = storage->artifact.subStats.at(subStatIndex)
													 .and_then([](const StatValue &val) -> std::optional<float> {
														 return val.value;
													 })
													 .value_or(0.f)
									};
								});
								subStatDisable.notify(!newStat.has_value());
								subStatValueUpdater.notify(
									storage->artifact.subStats.at(subStatIndex)
										.and_then([newStat, prevStat](const StatValue &stat) -> std::optional<float> {
											return stat.value * (Stats::isPercentage(prevStat.value_or(newStat.value())) ? 100.f : 1.f);
										})
										.value_or(0.f)
								);
								subStatTextUpdater.notify(
									newStat
										.transform([](auto &&val) {
											return Utils::Stringify(val);
										})
										.value_or("None")
								);
							}));
						},
					},
					.style = ButtonStyle::Standard(),
					.text = storage->artifact.subStats.at(subStatIndex).transform([](auto &&val) {
																		   return Utils::Stringify(val.stat);
																	   })
								.value_or("None"),
					.items = subStatItemsProvider(),
					.textUpdater = subStatTextUpdater,
				},
				NumberBox{
					.value = storage->artifact.subStats.at(subStatIndex).transform([&](auto &&val) {
																			return val.value * (Stats::isPercentage(storage->artifact.subStats.at(subStatIndex)->stat) ? 100.f : 1.f);
																		})
								 .value_or(0.f),
					.min = 0.f,
					.disabled = !storage->artifact.subStats.at(subStatIndex).has_value(),
					.onChange = [storage, subStatIndex](float val) {
						if (storage->artifact.subStats.at(subStatIndex).has_value()) {
							storage->artifact.subStats.at(subStatIndex)->value
								= val / (Stats::isPercentage(storage->artifact.subStats.at(subStatIndex)->stat) ? 100.f : 1.f);
						}
					},
					.formatter = [storage, subStatIndex](float val) {
						if (
							!storage->artifact.subStats.at(subStatIndex).has_value()
							|| !Stats::isPercentage(storage->artifact.subStats.at(subStatIndex)->stat)
						) {
							return std::format("{:.0f}", val);
						}
						return std::format("{:.1f}", val);
					},
					.valueUpdater = subStatValueUpdater,
					.controller{
						.disable = subStatDisable,
					},
				},
			},
		};
		return subStat;
	};
	auto subStat1 = createSubStat(0);
	auto subStat2 = createSubStat(1);
	auto subStat3 = createSubStat(2);
	auto subStat4 = createSubStat(3);

	Child content = Box{
		.color{1.f, 1.f, 1.f, 0.0538f},
		.borderColor{0.f, 0.f, 0.f, 0.1f},
		.borderWidth{0.f, 0.f, 1.f, 0.f},
		.borderRadius{7.f, 7.f, 0.f, 0.f},
		.borderPosition = squi::Box::BorderPosition::outset,
		.child = Column{
			.widget{
				.padding = 24.f,
			},
			.spacing = 16.f,
			.children{
				Container{
					.child = Text{
						.text = "Edit artifact",
						.fontSize = 24.f,
						.font = FontStore::defaultFontBold,
					},
				},
				levelSelector,
				setSelector,
				slotSelector,
				mainStatSelector,
				subStat1,
				subStat2,
				subStat3,
				subStat4,
			},
		},
	};

	Child buttonFooter = Box{
		.widget{
			.padding = 24.f,
		},
		.color{0x00000000},
		.borderRadius{0.f, 0.f, 7.f, 7.f},
		.child = Row{
			.spacing = 8.f,
			.children{
				Button{
					.widget{.width = Size::Expand},
					.text = "Save",
					.style = ButtonStyle::Accent(),
					.onClick = [closeEvent](GestureDetector::Event) {
						// FIXME: return the new artifact
						closeEvent.notify();
					},
				},
				Button{
					.widget{.width = Size::Expand},
					.text = "Cancel",
					.style = ButtonStyle::Standard(),
					.onClick = [closeEvent](GestureDetector::Event) {
						closeEvent.notify();
					},
				},
			},
		},
	};

	return Stack{
		.widget{
			.onInit = [closeEvent](Widget &w) {
				w.customState.add(closeEvent.observe([&w]() {
					w.deleteLater();
				}));
			},
		},
		.children{
			GestureDetector{
				.onClick = [closeEvent](GestureDetector::Event) {
					closeEvent.notify();
				},
				.child = Box{
					.color{0.f, 0.f, 0.f, 0.4f},
				},
			},
			Align{
				.child = Box{
					.widget{
						.height = Size::Shrink,
						.sizeConstraints{
							.maxWidth = 450.f,
						},
						.padding = 1.f,
					},
					.color = 0x202020FF,
					.borderColor{0.46f, 0.46f, 0.46f, 0.4f},
					.borderWidth{1.f},
					.borderRadius{8.f},
					.borderPosition = squi::Box::BorderPosition::outset,
					.child = Column{
						.children{
							content,
							buttonFooter,
						},
					},
				},
			},
		},
	};
}
