#include "artifactEditor.hpp"

#include "Ui/utils/editorItem.hpp"
#include "artifact/sets.hpp"
#include "dialog.hpp"
#include "stats/artifact.hpp"

#include "align.hpp"
#include "column.hpp"
#include "container.hpp"
#include "dropdownButton.hpp"
#include "gestureDetector.hpp"
#include "numberBox.hpp"
#include "observer.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "teachingTip.hpp"
#include "text.hpp"

#include "ranges"

using namespace squi;

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
	Child levelSelector = EditorItem{
		.name = "Level",
		.child = NumberBox{
			.value = static_cast<float>(storage->artifact.level),
			.min = 0.f,
			.max = 20.f,
			.onChange = [storage](float newVal) {
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
				.content = [&set = set.second, setChangeEvent]() {
					setChangeEvent.notify(set);
				},
			});
		}
		return ret;
	};
	Child setButton = DropdownButton{
		.widget{
			.onInit = [setChangeEvent, setTextUpdater, setItemsProvider, storage](Widget &w) {
				w.customState.add(setChangeEvent.observe([setTextUpdater, setItemsProvider, storage](const Artifact::Set &set) {
					storage->artifact.set = set.key;
					setTextUpdater.notify(std::string{set.name});
				}));
			},
		},
		.style = ButtonStyle::Standard(),
		.text = storage->artifact.set.key != 0 ? Artifact::sets.at(storage->artifact.set).name : "No set",
		.items = setItemsProvider(),
		.textUpdater = setTextUpdater,
	};
	Child setSelector = UI::EditorItem{
		.name = "Set",
		.child = setButton,
	};

	// Slot
	decltype(DropdownButton::textUpdater) slotTextUpdater{};
	Child slotSelector = UI::EditorItem{
		.name = "Slot",
		.child = DropdownButton{
			.widget{
				.onInit = [slotChangeEvent, slotTextUpdater, storage](Widget &w) {
					w.customState.add(slotChangeEvent.observe([slotTextUpdater, storage](Artifact::Slot newSlot) {
						storage->artifact.slot = newSlot;
						slotTextUpdater.notify(Utils::Stringify(newSlot));
					}));
				},
			},
			.style = ButtonStyle::Standard(),
			.text = Utils::Stringify(storage->artifact.slot),
			.disabled = (storage->artifact.equippedCharacter.key != 0),
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
	Child mainStatSelector = UI::EditorItem{
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
			auto filter = [&storage](auto &&val) {
				return storage->artifact.mainStat != val;
			};
			for (const auto &stat: std::views::filter(Stats::Artifact::subStats, filter)) {
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
								auto &subStat = storage->artifact.subStats.at(subStatIndex);
								subStat.stat = newStat;
								subStatDisable.notify(!newStat.has_value());
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
					.text = storage->artifact.subStats.at(subStatIndex).stat.transform([](auto &&val) {
																				return Utils::Stringify(val);
																			})
								.value_or("None"),
					.items = subStatItemsProvider(),
					.textUpdater = subStatTextUpdater,
				},
				NumberBox{
					.value = storage->artifact.subStats.at(subStatIndex).value * (Utils::isPercentage(storage->artifact.subStats.at(subStatIndex).stat) ? 100.f : 1.f),
					.min = 0.f,
					.disabled = !storage->artifact.subStats.at(subStatIndex).stat.has_value(),
					.onChange = [storage, subStatIndex](float val) {
						if (storage->artifact.subStats.at(subStatIndex).stat.has_value()) {
							storage->artifact.subStats.at(subStatIndex).value = val / (Utils::isPercentage(storage->artifact.subStats.at(subStatIndex).stat) ? 100.f : 1.f);
						}
					},
					.formatter = [storage, subStatIndex](float val) {
						if (
							!storage->artifact.subStats.at(subStatIndex).stat.has_value()
							|| !Utils::isPercentage(storage->artifact.subStats.at(subStatIndex).stat.value())
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

	auto content = Column{
		.spacing = 16.f,
		.children{
			levelSelector,
			setSelector,
			slotSelector,
			mainStatSelector,
			subStat1,
			subStat2,
			subStat3,
			subStat4,
		},
	};

	Children buttonFooter{
		Button{
			.widget{.width = Size::Expand},
			.text = "Save",
			.style = ButtonStyle::Accent(),
			.onClick = [closeEvent, onSubmit = onSubmit, setButton = std::weak_ptr(setButton), storage](GestureDetector::Event event) {
				if (storage->artifact.set.key == 0) {
					event.widget.addOverlay(TeachingTip{
						.target = setButton,
						.message = "You must specify a set",
					});
					return;
				}
				closeEvent.notify();
				if (onSubmit) onSubmit(storage->artifact);
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
	};

	return Dialog{
		.closeEvent = closeEvent,
		.title = "Edit artifact",
		.content = content,
		.buttons = buttonFooter,
	};
}
