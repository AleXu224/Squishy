#include "characterEditor.hpp"

#include "Ui/utils/editorItem.hpp"
#include "character/data.hpp"
#include "misc/ascension.hpp"
#include "observer.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/dialog.hpp"
#include "widgets/dropdownButton.hpp"
#include "widgets/numberBox.hpp"
#include "widgets/row.hpp"
#include "widgets/text.hpp"
#include <algorithm>
#include <array>

using namespace squi;
using TalentType = Talents<Stats::Value<int32_t, 1>>;
void UI::CharacterEditor::State::clampAscension() {
	auto rarity = character->state.stats.data.baseStats.rarity;
	auto availableAscensions = Misc::ascensionsAtLvl(character->state.stats.sheet.level, rarity);
	if (availableAscensions.empty()) {
		std::println("Got bad lvl {}", character->state.stats.sheet.level);
		return;
	}
	if (std::ranges::none_of(availableAscensions, [&](const Misc::Ascension &ascension) {
			return ascension.ascension == character->state.stats.sheet.ascension;
		})) {
		setState([&]() {
			character->state.stats.sheet.ascension = availableAscensions.front().ascension;
		});
	}
}

void UI::CharacterEditor::State::clampTalents() {
	const auto &ascension = Misc::ascensions.at(character->state.stats.sheet.ascension);
	constexpr auto talents = std::array{
		&TalentType::normal,
		&TalentType::skill,
		&TalentType::burst,
	};
	for (const auto &talentPtr: talents) {
		auto &talentLvl = std::invoke(talentPtr, character->state.stats.sheet.talents).constant;


		auto newTalentLvl = std::min(static_cast<int32_t>(ascension.maxTalent) - 1, talentLvl);
		if (newTalentLvl != talentLvl) {
			setState([&]() {
				talentLvl = newTalentLvl;
			});
		}
	}
}

squi::core::Child UI::CharacterEditor::State::build(const Element &element) {
	// Level
	auto levelSelector = NumberBox{
		.widget{
			.width = 40.f,
		},
		.value = static_cast<float>(character->state.stats.sheet.level),
		.min = 1.f,
		.max = static_cast<float>(Misc::ascensions.at(Misc::maxAscensionByRarity.at(character->state.stats.data.baseStats.rarity)).maxLevel),
		.precision = 0,
		.onChange = [this](float newVal) {
			setState([&]() {
				character->state.stats.sheet.level = std::floor(newVal);
			});
			clampAscension();
			clampTalents();
		},
	};

	// Ascension
	auto ascensionItems = [this]() {
		std::vector<ContextMenu::Item> ret{};
		auto rarity = character->state.stats.data.baseStats.rarity;
		for (const auto &ascension: Misc::ascensionsAtLvl(character->state.stats.sheet.level, rarity)) {
			ret.push_back(ContextMenu::Button{
				.text = fmt::format("{}", ascension.maxLevel),
				.callback = [this, ascension]() {
					setState([&]() {
						character->state.stats.sheet.ascension = ascension.ascension;
					});
					clampTalents();
				},
			});
		}
		return ret;
	}();
	auto ascensionSelector = DropdownButton{
		.theme = Button::Theme::Standard(),
		.disabled = ascensionItems.size() <= 1,
		.text = fmt::format("{}", Misc::ascensions.at(character->state.stats.sheet.ascension).maxLevel),
		.items = ascensionItems,
	};

	auto levelAscensionSeparator = Container{
		.widget{
			.width = Size::Shrink,
			.padding = Padding{4.f, 0.f},
		},
		.child = Text{
			.widget{
				.alignment = Alignment::Center,
			},
			.text = "/",
		},
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
	auto constellationSelector = UI::EditorItem{
		.name = "Constellation",
		.child = DropdownButton{
			.theme = Button::Theme::Standard(),
			.text = fmt::format("Constellation {}", character->state.stats.sheet.constellation),
			.items = [this]() {
				std::vector<ContextMenu::Item> ret{};
				for (const auto &constellation: std::views::iota(0, 7)) {
					ret.emplace_back(ContextMenu::Button{
						.text = fmt::format("Constellation {}", constellation),
						.callback = [this, constellation]() {
							setState([&]() {
								character->state.stats.sheet.constellation = constellation;
							});
						},
					});
				}
				return ret;
			}(),
		},
	};

	// Talent
	auto getTalentSelector = [&](std::string_view name, Stats::Value<int32_t, 1> TalentType::*talentPtr) -> Child {
		return UI::EditorItem{
			.name = std::string(name),
			.child = DropdownButton{
				.theme = Button::Theme::Standard(),
				.text = fmt::format("Lvl {}", std::invoke(talentPtr, character->state.stats.sheet.talents).constant + 1),
				.items = [this, talentPtr]() {
					std::vector<ContextMenu::Item> ret{};
					const auto &ascension = Misc::ascensions.at(character->state.stats.sheet.ascension);
					for (const auto &talentLvl: std::views::iota(static_cast<uint8_t>(0), ascension.maxTalent)) {
						ret.emplace_back(ContextMenu::Button{
							.text = fmt::format("Lvl {}", talentLvl + 1),
							.callback = [this, talentLvl, talentPtr]() {
								setState([&]() {
									std::invoke(talentPtr, character->state.stats.sheet.talents).constant = talentLvl;
								});
							},
						});
					}
					return ret;
				}(),
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
			.theme = Button::Theme::Accent(),
			.onClick = [this]() {
				closeEvent.notify();
				if (widget->onSubmit) widget->onSubmit(*character);
			},
			.child = "Save",
		},
		Button{
			.widget{.width = Size::Expand},
			.theme = Button::Theme::Standard(),
			.onClick = [this]() {
				closeEvent.notify();
			},
			.child = "Cancel",
		},
	};


	return Dialog{
		.closeEvent = closeEvent,
		.title = "Edit character",
		.content = content,
		.buttons = buttonFooter,
	};
}
