#include "engineEditor.hpp"

#include "UI/utils/editorItem.hpp"
#include "engine/engines.hpp"
#include "engineSelector.hpp"
#include "misc/promotion.hpp"
#include "observer.hpp"
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

squi::core::Child UI::EngineEditor::State::build(const Element &element) {
	// Engine
	Child engineSelector = UI::EditorItem{
		.name = "Engine",
		.child = Button{
			.theme = Button::Theme::Standard(),
			.onClick = [this]() {
				Navigator::of(*this->element).pushOverlay(EngineSelector{
					.specialty = engine.stats.data->baseStats.specialty,
					.onSelect = [this](Engine::DataKey key) {
						auto level = engine.stats.sheet.level;
						auto modification = engine.stats.sheet.modification;
						auto overclock = engine.stats.sheet.overclock;

						auto rarity = Engine::list.at(key).baseStats.rarity;
						auto maxModification = Misc::maxPromotionByRarity.at(rarity);
						auto maxLvl = Misc::promotions.at(maxModification).maxLevel;


						setState([&]() {
							engine.stats = ::Stats::Engine(::Engine::list.at(key));
							engine.stats.sheet.level = level;
							engine.stats.sheet.modification = modification;
							engine.stats.sheet.overclock = rarity >= 2 ? overclock : 1;

							if (engine.stats.sheet.level > maxLvl)
								engine.stats.sheet.level = maxLvl;
							if (engine.stats.sheet.modification > maxModification)
								engine.stats.sheet.modification = maxModification;
						});
					},
				});
			},
			.child = engine.stats.data->name,
		},
	};

	// Level
	Child levelSelector = NumberBox{
		.widget{
			.width = 40.f,
		},
		.value = static_cast<float>(engine.stats.sheet.level),
		.min = 1.f,
		.max = static_cast<float>(Misc::promotions.at(Misc::maxPromotionByRarity.at(engine.stats.data->baseStats.rarity)).maxLevel),
		.precision = 0,
		.onChange = [this](float newVal) {
			setState([&]() {
				engine.stats.sheet.level = std::floor(newVal);

				auto modification = Misc::promotions.at(engine.stats.sheet.modification);
				if (engine.stats.sheet.level > modification.maxLevel || engine.stats.sheet.level < modification.minLevel) {
					engine.stats.sheet.modification = Misc::promotionsAtLvl(engine.stats.sheet.level, engine.stats.data->baseStats.rarity).front().promotion;
				}
			});
		},
	};

	// Promotion
	auto promotionItemFactory = [this]() {
		std::vector<ContextMenu::Item> ret{};
		auto rarity = engine.stats.data->baseStats.rarity;
		for (const auto &promotion: Misc::promotionsAtLvl(engine.stats.sheet.level, rarity)) {
			ret.emplace_back(ContextMenu::Button{
				.text = std::format("{}", promotion.maxLevel),
				.callback = [this, promotion]() {
					setState([&]() {
						engine.stats.sheet.modification = promotion.promotion;

						if (engine.stats.sheet.level > promotion.maxLevel)
							engine.stats.sheet.level = promotion.maxLevel;
						if (engine.stats.sheet.level < promotion.minLevel)
							engine.stats.sheet.level = promotion.minLevel;
					});
				},
			});
		}
		return ret;
	};
	auto promotionSelectorText = std::format("{}", Misc::promotions.at(engine.stats.sheet.modification).maxLevel);
	Child promotionSelector = DropdownButton{
		.theme = Button::Theme::Standard(),
		.disabled = promotionItemFactory().size() <= 1,
		.text = promotionSelectorText,
		.items = promotionItemFactory(),
	};

	Child levelPromotionSeparator = Container{
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

	Child levelModificationSelector = UI::EditorItem{
		.name = "Level",
		.child = Row{
			.widget{
				.width = Size::Shrink,
			},
			.children{
				levelSelector,
				levelPromotionSeparator,
				promotionSelector,
			},
		},
	};

	// Overclock
	Child overclockSelector = UI::EditorItem{
		.name = "Overclock",
		.child = DropdownButton{
			.theme = Button::Theme::Standard(),
			.disabled = engine.stats.data->baseStats.rarity <= 2,
			.text = std::format("{}", engine.stats.sheet.overclock),
			.items = [this]() {
				std::vector<ContextMenu::Item> ret{};
				for (const auto &overclock: std::views::iota(1, 6)) {
					ret.emplace_back(ContextMenu::Button{
						.text = std::format("Overclock {}", overclock),
						.callback = [this, overclock]() {
							setState([&]() {
								engine.stats.sheet.overclock = overclock;
							});
						},
					});
				}
				return ret;
			}(),
		},
	};

	Child content = Column{
		.spacing = 16.f,
		.children{
			engineSelector,
			levelModificationSelector,
			overclockSelector,
		},
	};

	Children buttonFooter{
		Button{
			.widget{.width = Size::Expand},
			.theme = Button::Theme::Accent(element),
			.onClick = [this]() {
				closeEvent.notify();
				if (widget->onSubmit) widget->onSubmit(engine);
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
		.title = "Edit engine",
		.content = content,
		.buttons = buttonFooter,
	};
}
