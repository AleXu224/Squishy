#include "weaponEditor.hpp"

#include "Ui/utils/editorItem.hpp"
#include "misc/ascension.hpp"
#include "observer.hpp"
#include "weapon/weapons.hpp"
#include "weaponSelector.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/dialog.hpp"
#include "widgets/dropdownButton.hpp"
#include "widgets/navigator.hpp"
#include "widgets/numberBox.hpp"
#include "widgets/row.hpp"
#include "widgets/text.hpp"

using namespace squi;

squi::core::Child UI::WeaponEditor::State::build(const Element &element) {
	// Weapon
	auto weaponSelector = UI::EditorItem{
		.name = "Weapon",
		.child = Button{
			.theme = Button::Theme::Standard(),
			.onClick = [this]() {
				Navigator::of(*this->element).pushOverlay(WeaponSelector{
					.type = weapon.stats.data->baseStats.type,
					.onSelect = [this](Weapon::DataKey key) {
						auto level = weapon.stats.sheet.level;
						auto ascension = weapon.stats.sheet.ascension;
						auto refinement = weapon.stats.sheet.refinement;

						auto rarity = Weapon::list.at(key).baseStats.rarity;
						auto maxAscension = Misc::maxAscensionByRarity.at(rarity);
						auto maxLvl = Misc::ascensions.at(maxAscension).maxLevel;


						setState([&]() {
							weapon.stats = ::Stats::Weapon(::Weapon::list.at(key));
							weapon.stats.sheet.level = level;
							weapon.stats.sheet.ascension = ascension;
							weapon.stats.sheet.refinement = rarity >= 2 ? refinement : 1;

							if (weapon.stats.sheet.level > maxLvl)
								weapon.stats.sheet.level = maxLvl;
							if (weapon.stats.sheet.ascension > maxAscension)
								weapon.stats.sheet.ascension = maxAscension;
						});
					},
				});
			},
			.child = weapon.stats.data->name,
		},
	};

	// Level
	auto levelSelector = NumberBox{
		.widget{
			.width = 40.f,
		},
		.value = static_cast<float>(weapon.stats.sheet.level),
		.min = 1.f,
		.max = static_cast<float>(Misc::ascensions.at(Misc::maxAscensionByRarity.at(weapon.stats.data->baseStats.rarity)).maxLevel),
		.precision = 0,
		.onChange = [this](float newVal) {
			setState([&]() {
				weapon.stats.sheet.level = std::floor(newVal);

				auto ascension = Misc::ascensions.at(weapon.stats.sheet.ascension);
				if (weapon.stats.sheet.level > ascension.maxLevel || weapon.stats.sheet.level < ascension.minLevel) {
					weapon.stats.sheet.ascension = Misc::ascensionsAtLvl(weapon.stats.sheet.level, weapon.stats.data->baseStats.rarity).front().ascension;
				}
			});
		},
	};

	// Ascension
	auto ascensionItemFactory = [this]() {
		std::vector<ContextMenu::Item> ret{};
		auto rarity = weapon.stats.data->baseStats.rarity;
		for (const auto &ascension: Misc::ascensionsAtLvl(weapon.stats.sheet.level, rarity)) {
			ret.emplace_back(ContextMenu::Button{
				.text = fmt::format("{}", ascension.maxLevel),
				.callback = [this, ascension]() {
					setState([&]() {
						weapon.stats.sheet.ascension = ascension.ascension;

						if (weapon.stats.sheet.level > ascension.maxLevel)
							weapon.stats.sheet.level = ascension.maxLevel;
						if (weapon.stats.sheet.level < ascension.minLevel)
							weapon.stats.sheet.level = ascension.minLevel;
					});
				},
			});
		}
		return ret;
	};
	auto ascensionSelectorText = fmt::format("{}", Misc::ascensions.at(weapon.stats.sheet.ascension).maxLevel);
	auto ascensionSelector = DropdownButton{
		.theme = Button::Theme::Standard(),
		.disabled = ascensionItemFactory().size() <= 1,
		.text = ascensionSelectorText,
		.items = ascensionItemFactory(),
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

	// Refinement
	auto refinementSelector = UI::EditorItem{
		.name = "Refinement",
		.child = DropdownButton{
			.theme = Button::Theme::Standard(),
			.disabled = weapon.stats.data->baseStats.rarity <= 2,
			.text = fmt::format("{}", weapon.stats.sheet.refinement),
			.items = [this]() {
				std::vector<ContextMenu::Item> ret{};
				for (const auto &refinement: std::views::iota(1, 6)) {
					ret.emplace_back(ContextMenu::Button{
						.text = fmt::format("Refinement {}", refinement),
						.callback = [this, refinement]() {
							setState([&]() {
								weapon.stats.sheet.refinement = refinement;
							});
						},
					});
				}
				return ret;
			}(),
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
			.theme = Button::Theme::Accent(),
			.onClick = [this]() {
				closeEvent.notify();
				if (widget->onSubmit) widget->onSubmit(weapon);
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
		.title = "Edit weapon",
		.content = content,
		.buttons = buttonFooter,
	};
}
