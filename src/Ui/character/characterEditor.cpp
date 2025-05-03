#include "characterEditor.hpp"

#include "Ui/utils/editorItem.hpp"
#include "align.hpp"
#include "button.hpp"
#include "character/data.hpp"
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
#include <algorithm>


using namespace squi;

UI::CharacterEditor::operator Child() const {
	auto storage = std::make_shared<Storage>(character);

	VoidObservable closeEvent{};
	Observable<uint8_t> levelUpdateEvent{};
	Observable<uint8_t> ascensionUpdateEvent{};
	Observable<uint8_t> constellationUpdateEvent{};

	// Level
	auto levelSelector = NumberBox{
		.widget{
			.width = 40.f,
		},
		.value = static_cast<float>(storage->character.state.stats.sheet.level),
		.min = 1.f,
		.max = static_cast<float>(Misc::ascensions.at(Misc::maxAscensionByRarity.at(storage->character.state.stats.data.baseStats.rarity)).maxLevel),
		.onChange = [storage, levelUpdateEvent](float newVal) {
			storage->character.state.stats.sheet.level = std::floor(newVal);
			levelUpdateEvent.notify(storage->character.state.stats.sheet.level);
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
		auto rarity = storage->character.state.stats.data.baseStats.rarity;
		for (const auto &ascension: Misc::ascensionsAtLvl(storage->character.state.stats.sheet.level, rarity)) {
			ret.emplace_back(ContextMenu::Item{
				.text = fmt::format("{}", ascension.maxLevel),
				.content = [ascension, ascensionUpdateEvent]() {
					ascensionUpdateEvent.notify(ascension.ascension);
				},
			});
		}
		return ret;
	};
	auto ascensionSelectorText = fmt::format("{}", Misc::ascensions.at(storage->character.state.stats.sheet.ascension).maxLevel);
	auto ascensionSelector = DropdownButton{
		.widget{
			.onInit = [ascensionUpdateEvent, ascensionTextUpdater, storage, levelUpdateEvent, ascensionItemUpdater, ascensionItemFactory](Widget &w) {
				w.customState.add("levelUpdateEvent", levelUpdateEvent.observe([storage, ascensionItemUpdater, ascensionItemFactory, ascensionUpdateEvent, &w](uint8_t level) {
					auto rarity = storage->character.state.stats.data.baseStats.rarity;
					auto availableAscensions = Misc::ascensionsAtLvl(level, rarity);
					if (availableAscensions.empty()) {
						std::println("Got bad lvl {}", level);
						return;
					}
					ascensionItemUpdater.notify(ascensionItemFactory());
					if (std::ranges::none_of(availableAscensions, [&](const Misc::Ascension &ascension) {
							return ascension.ascension == storage->character.state.stats.sheet.ascension;
						})) {
						ascensionUpdateEvent.notify(availableAscensions.front().ascension);
					}
					w.customState.get<bool>("disabled") = availableAscensions.size() == 1;
				}));
				w.customState.add("ascensionUpdateEvent", ascensionUpdateEvent.observe([ascensionTextUpdater, storage](uint8_t newAscension) {
					storage->character.state.stats.sheet.ascension = newAscension;
					ascensionTextUpdater.notify(fmt::format("{}", Misc::ascensions.at(storage->character.state.stats.sheet.ascension).maxLevel));
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

	// Constellation
	decltype(DropdownButton::textUpdater) constellationTextUpdater{};
	auto constellationSelector = UI::EditorItem{
		.name = "Constellation",
		.child = DropdownButton{
			.widget{
				.onInit = [constellationUpdateEvent, storage, constellationTextUpdater](Widget &w) {
					observe(w, constellationUpdateEvent, [storage, constellationTextUpdater](uint8_t newConstellation) {
						storage->character.state.stats.sheet.constellation = newConstellation;
						constellationTextUpdater.notify(fmt::format("Constellation {}", newConstellation));
					});
				},
			},
			.style = ButtonStyle::Standard(),
			.text = fmt::format("Constellation {}", storage->character.state.stats.sheet.constellation),
			.items = [storage, constellationUpdateEvent]() {
				std::vector<ContextMenu::Item> ret{};
				for (const auto &constellation: std::views::iota(0, 7)) {
					ret.emplace_back(ContextMenu::Item{
						.text = fmt::format("Constellation {}", constellation),
						.content = [constellation, constellationUpdateEvent]() {
							constellationUpdateEvent.notify(constellation);
						},
					});
				}
				return ret;
			}(),
			.textUpdater = constellationTextUpdater,
		},
	};

	// Talent
	using TalentType = Talents<Stats::Value<int32_t, 1>>;
	auto getTalentSelector = [&](std::string_view name, Stats::Value<int32_t, 1> TalentType::*talentPtr) {
		decltype(DropdownButton::textUpdater) talentTextUpdater{};
		decltype(DropdownButton::itemsUpdater) talentItemUpdater{};
		Observable<uint8_t> talentUpdateEvent{};

		auto itemBuilder = [storage, talentUpdateEvent]() {
			std::vector<ContextMenu::Item> ret{};
			const auto &ascension = Misc::ascensions.at(storage->character.state.stats.sheet.ascension);
			for (const auto &talentLvl: std::views::iota(static_cast<uint8_t>(0), ascension.maxTalent)) {
				ret.emplace_back(ContextMenu::Item{
					.text = fmt::format("Lvl {}", talentLvl + 1),
					.content = [talentLvl, talentUpdateEvent]() {
						talentUpdateEvent.notify(talentLvl);
					},
				});
			}
			return ret;
		};

		return UI::EditorItem{
			.name = name,
			.child = DropdownButton{
				.widget{
					.onInit = [talentUpdateEvent, storage, talentTextUpdater, talentPtr, ascensionUpdateEvent, talentItemUpdater, itemBuilder, name](Widget &w) {
						observe(w, talentUpdateEvent, [storage, talentTextUpdater, talentPtr](uint8_t newTalent) {
							std::invoke(talentPtr, storage->character.state.stats.sheet.talents).constant = newTalent;
							talentTextUpdater.notify(fmt::format("Lvl {}", newTalent + 1));
						});

						observe(name, w, ascensionUpdateEvent, [talentPtr, storage, talentUpdateEvent, talentItemUpdater, itemBuilder](uint8_t newAscension) {
							const auto &ascension = Misc::ascensions.at(newAscension);
							auto &talentLvl = std::invoke(talentPtr, storage->character.state.stats.sheet.talents).constant;

							talentLvl = std::min(static_cast<int32_t>(ascension.maxTalent) - 1, talentLvl);
							talentUpdateEvent.notify(talentLvl);
							talentItemUpdater.notify(itemBuilder());
						});
					},
				},
				.style = ButtonStyle::Standard(),
				.text = fmt::format("Lvl {}", std::invoke(talentPtr, storage->character.state.stats.sheet.talents).constant + 1),
				.items = itemBuilder(),
				.textUpdater = talentTextUpdater,
				.itemsUpdater = talentItemUpdater,
			},
		};
	};

	auto content = Column{
		.spacing = 16.f,
		.children{
			levelAscensionSelector,
			constellationSelector,
			getTalentSelector("Normal Attack", &TalentType::normal),
			getTalentSelector("Elemental Skill", &TalentType::skill),
			getTalentSelector("Elemental Burst", &TalentType::burst),
		},
	};

	Children buttonFooter{
		Button{
			.widget{.width = Size::Expand},
			.text = "Save",
			.style = ButtonStyle::Accent(),
			.onClick = [closeEvent, storage, onSubmit = onSubmit](GestureDetector::Event) {
				closeEvent.notify();
				if (onSubmit) onSubmit(storage->character);
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
		.title = "Edit character",
		.content = content,
		.buttons = buttonFooter,
	};
}