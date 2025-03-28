#include "weaponEditor.hpp"

#include "Ui/utils/editorItem.hpp"
#include "align.hpp"
#include "button.hpp"
#include "column.hpp"
#include "container.hpp"
#include "dialog.hpp"
#include "dropdownButton.hpp"
#include "expander.hpp"
#include "gestureDetector.hpp"
#include "misc/ascension.hpp"
#include "numberBox.hpp"
#include "observer.hpp"
#include "row.hpp"
#include "text.hpp"
#include "weapon/weapons.hpp"
#include "weaponSelector.hpp"
#include <algorithm>

using namespace squi;

UI::WeaponEditor::operator Child() const {
	auto storage = std::make_shared<Storage>(weapon);

	VoidObservable closeEvent{};
	Observable<uint8_t> levelUpdateEvent{};
	Observable<uint8_t> ascensionUpdateEvent{};
	Observable<uint8_t> refinementUpdateEvent{};
	Observable<Weapon::DataKey> weaponUpdateEvent{};

	// Weapon
	auto weaponSelector = UI::EditorItem{
		.name = "Weapon",
		.child = Button{
			.widget{
				.onInit = [weaponUpdateEvent](Widget &w) {
					observe(w, weaponUpdateEvent, [&w](Weapon::DataKey key) {
						w.customState.get<Observable<std::string>>("updateText").notify(Weapon::list.at(key).name);
					});
				},
			},
			.text = storage->weapon.stats.data->name,
			.style = ButtonStyle::Standard(),
			.onClick = [weaponUpdateEvent, storage](GestureDetector::Event event) {
				event.widget.addOverlay(WeaponSelector{
					.type = storage->weapon.stats.data->baseStats.type,
					.onSelect = [weaponUpdateEvent, storage](Weapon::DataKey key) {
						auto level = storage->weapon.stats.sheet.level;
						auto ascension = storage->weapon.stats.sheet.ascension;
						auto refinement = storage->weapon.stats.sheet.refinement;

						storage->weapon.stats = ::Stats::Weapon(::Weapon::list.at(key));
						storage->weapon.stats.sheet.level = level;
						storage->weapon.stats.sheet.ascension = ascension;
						storage->weapon.stats.sheet.refinement = refinement;
						weaponUpdateEvent.notify(key);
					},
				});
			},
		},
	};

	// Level
	auto levelSelector = NumberBox{
		.widget{
			.width = 40.f,
			.onInit = [weaponUpdateEvent](Widget &w) {
				observe(w, weaponUpdateEvent, [&w](Weapon::DataKey key) {
					auto rarity = Weapon::list.at(key).baseStats.rarity;
					auto maxLvl = Misc::ascensions.at(Misc::maxAscensionByRarity.at(rarity)).maxLevel;
					NumberBox::State::max.of(w) << maxLvl;
				});
			},
		},
		.value = static_cast<float>(storage->weapon.stats.sheet.level),
		.min = 1.f,
		.max = static_cast<float>(Misc::ascensions.at(Misc::maxAscensionByRarity.at(storage->weapon.stats.data->baseStats.rarity)).maxLevel),
		.onChange = [storage, levelUpdateEvent](float newVal) {
			storage->weapon.stats.sheet.level = std::floor(newVal);
			levelUpdateEvent.notify(storage->weapon.stats.sheet.level);
		},
		.validator = [](float val) {
			return TextBox::Controller::ValidatorResponse{
				.valid = (val == std::round(val)),
				.message = "Value must be an intereger",
			};
		},
	};

	// Ascension
	decltype(DropdownButton::textUpdater) ascensionTextUpdater{};
	decltype(DropdownButton::itemsUpdater) ascensionItemUpdater{};
	auto ascensionItemFactory = [storage, ascensionUpdateEvent]() {
		std::vector<ContextMenu::Item> ret{};
		auto rarity = storage->weapon.stats.data->baseStats.rarity;
		for (const auto &ascension: Misc::ascensionsAtLvl(storage->weapon.stats.sheet.level, rarity)) {
			ret.emplace_back(ContextMenu::Item{
				.text = fmt::format("{}", ascension.maxLevel),
				.content = [ascension, ascensionUpdateEvent]() {
					ascensionUpdateEvent.notify(ascension.ascension);
				},
			});
		}
		return ret;
	};
	auto ascensionSelectorText = fmt::format("{}", Misc::ascensions.at(storage->weapon.stats.sheet.ascension).maxLevel);
	auto ascensionSelector = DropdownButton{
		.widget{
			.onInit = [ascensionUpdateEvent, ascensionTextUpdater, storage, levelUpdateEvent, ascensionItemUpdater, ascensionItemFactory](Widget &w) {
				w.customState.add("levelUpdateEvent", levelUpdateEvent.observe([storage, ascensionItemUpdater, ascensionItemFactory, ascensionUpdateEvent, &w](uint8_t level) {
					auto rarity = storage->weapon.stats.data->baseStats.rarity;
					auto availableAscensions = Misc::ascensionsAtLvl(level, rarity);
					if (availableAscensions.empty()) {
						std::println("Got bad lvl {}", level);
						return;
					}
					ascensionItemUpdater.notify(ascensionItemFactory());
					if (std::ranges::none_of(availableAscensions, [&](const Misc::Ascension &ascension) {
							return ascension.ascension == storage->weapon.stats.sheet.ascension;
						})) {
						ascensionUpdateEvent.notify(availableAscensions.front().ascension);
					}
					w.customState.get<bool>("disabled") = availableAscensions.size() == 1;
				}));
				w.customState.add("ascensionUpdateEvent", ascensionUpdateEvent.observe([ascensionTextUpdater, storage](uint8_t newAscension) {
					storage->weapon.stats.sheet.ascension = newAscension;
					ascensionTextUpdater.notify(fmt::format("{}", Misc::ascensions.at(storage->weapon.stats.sheet.ascension).maxLevel));
				}));
			},
		},
		.style = ButtonStyle::Standard(),
		.text = ascensionSelectorText,
		.disabled = ascensionItemFactory().size() <= 1,
		.items = ascensionItemFactory(),
		.textUpdater = ascensionTextUpdater,
		.itemsUpdater = ascensionItemUpdater,
	};

	auto levelAscensionSeparator = Container{
		.widget{
			.width = Size::Shrink,
			.padding = Padding{4.f, 0.f},
		},
		.child = Align{.child = Text{.text = "/"}},
	};

	auto levelAscensionSelector = UI::EditorItem{
		.name = "Level",
		.child = Row{
			.widget{
				.width = Size::Shrink,
			},
			.children{
				levelSelector,
				levelAscensionSeparator,
				ascensionSelector,
			},
		},
	};

	// Refinement
	// Ascension
	decltype(DropdownButton::textUpdater) refinementTextUpdater{};
	auto refinementSelector = UI::EditorItem{
		.widget{
			.onInit = [weaponUpdateEvent, storage](Widget &w) {
				observe(w, weaponUpdateEvent, [&w](Weapon::DataKey key) {
					auto rarity = Weapon::list.at(key).baseStats.rarity;
					w.flags.visible = (rarity > 2);
				});
				w.flags.visible = (storage->weapon.stats.data->baseStats.rarity > 2);
			},
		},
		.name = "Refinement",
		.child = DropdownButton{
			.widget{
				.onInit = [refinementUpdateEvent, storage, refinementTextUpdater, weaponUpdateEvent](Widget &w) {
					w.customState.add(refinementUpdateEvent.observe([storage, refinementTextUpdater](uint8_t newRefinement) {
						storage->weapon.stats.sheet.refinement = newRefinement;
						refinementTextUpdater.notify(fmt::format("{}", newRefinement));
					}));
				},
			},
			.style = ButtonStyle::Standard(),
			.text = fmt::format("{}", storage->weapon.stats.sheet.refinement),
			.items = [storage, ascensionUpdateEvent, refinementUpdateEvent]() {
				std::vector<ContextMenu::Item> ret{};
				for (const auto &refinement: std::views::iota(1, 6)) {
					ret.emplace_back(ContextMenu::Item{
						.text = fmt::format("Refinement {}", refinement),
						.content = [refinementUpdateEvent, refinement]() {
							refinementUpdateEvent.notify(refinement);
						},
					});
				}
				return ret;
			}(),
			.textUpdater = refinementTextUpdater,
		},
	};

	auto content = Column{
		.spacing = 16.f,
		.children{
			weaponSelector,
			levelAscensionSelector,
			refinementSelector,
		},
	};

	Children buttonFooter{
		Button{
			.widget{.width = Size::Expand},
			.text = "Save",
			.style = ButtonStyle::Accent(),
			.onClick = [closeEvent, storage, onSubmit = onSubmit](GestureDetector::Event) {
				closeEvent.notify();
				if (onSubmit) onSubmit(storage->weapon);
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
		.title = "Edit weapon",
		.content = content,
		.buttons = buttonFooter,
	};
}