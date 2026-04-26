#include "agentEditor.hpp"

#include "UI/utils/editorItem.hpp"
#include "agent/data.hpp"
#include "misc/promotion.hpp"
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
using SkillType = Skills<Stats::Value<int32_t, 2>>;
void UI::AgentEditor::State::clampPromotion() {
	auto rarity = agent->state.stats.data.baseStats.rarity;
	auto availablePromotions = Misc::promotionsAtLvl(agent->state.stats.sheet.level, rarity);
	if (availablePromotions.empty()) {
		std::println("Got bad lvl {}", agent->state.stats.sheet.level);
		return;
	}
	if (std::ranges::none_of(availablePromotions, [&](const Misc::Promotion &promotion) {
			return promotion.promotion == agent->state.stats.sheet.promotion;
		})) {
		setState([&]() {
			agent->state.stats.sheet.promotion = availablePromotions.front().promotion;
		});
	}
}

void UI::AgentEditor::State::clampSkills() {
	const auto &promotion = Misc::promotions.at(agent->state.stats.sheet.promotion);
	constexpr auto skills = std::array{
		&SkillType::basic,
		&SkillType::dodge,
		&SkillType::assist,
		&SkillType::special,
		&SkillType::chain,
		&SkillType::core,
	};
	for (const auto &skillPtr: skills) {
		auto &skillLvl = std::invoke(skillPtr, agent->state.stats.sheet.skills).constant;


		auto newSkillLvl = std::min(static_cast<int32_t>(promotion.maxSkill) - 1, skillLvl);
		if (newSkillLvl != skillLvl) {
			setState([&]() {
				skillLvl = newSkillLvl;
			});
		}
	}
}

squi::core::Child UI::AgentEditor::State::build(const Element &element) {
	// Level
	Child levelSelector = NumberBox{
		.widget{
			.width = 40.f,
		},
		.value = static_cast<float>(agent->state.stats.sheet.level),
		.min = 1.f,
		.max = static_cast<float>(Misc::promotions.at(Misc::maxPromotionByRarity.at(agent->state.stats.data.baseStats.rarity)).maxLevel),
		.precision = 0,
		.onChange = [this](float newVal) {
			setState([&]() {
				agent->state.stats.sheet.level = std::floor(newVal);
			});
			clampPromotion();
			clampSkills();
		},
	};

	// Promotion
	auto promotionItems = [this]() {
		std::vector<ContextMenu::Item> ret{};
		auto rarity = agent->state.stats.data.baseStats.rarity;
		for (const auto &promotion: Misc::promotionsAtLvl(agent->state.stats.sheet.level, rarity)) {
			ret.push_back(ContextMenu::Button{
				.text = fmt::format("{}", promotion.maxLevel),
				.callback = [this, promotion]() {
					setState([&]() {
						agent->state.stats.sheet.promotion = promotion.promotion;
					});
					clampSkills();
				},
			});
		}
		return ret;
	}();
	Child promotionSelector = DropdownButton{
		.theme = Button::Theme::Standard(),
		.disabled = promotionItems.size() <= 1,
		.text = fmt::format("{}", Misc::promotions.at(agent->state.stats.sheet.promotion).maxLevel),
		.items = promotionItems,
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

	Child levelPromotionSelector = UI::EditorItem{
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

	// Mindscape
	Child mindscapeSelector = UI::EditorItem{
		.name = "Mindscape",
		.child = DropdownButton{
			.theme = Button::Theme::Standard(),
			.text = fmt::format("Mindscape {}", agent->state.stats.sheet.mindscape),
			.items = [this]() {
				std::vector<ContextMenu::Item> ret{};
				for (const auto &mindscape: std::views::iota(0, 7)) {
					ret.emplace_back(ContextMenu::Button{
						.text = fmt::format("Mindscape {}", mindscape),
						.callback = [this, mindscape]() {
							setState([&]() {
								agent->state.stats.sheet.mindscape = mindscape;
							});
						},
					});
				}
				return ret;
			}(),
		},
	};

	// Skill
	auto getSkillSelector = [&](std::string_view name, Stats::Value<int32_t, 2> SkillType::*skillPtr, bool isCore = false) -> Child {
		return UI::EditorItem{
			.name = std::string(name),
			.child = DropdownButton{
				.theme = Button::Theme::Standard(),
				.text = fmt::format("Lvl {}", std::invoke(skillPtr, agent->state.stats.sheet.skills).constant + (isCore ? 0 : 1)),
				.items = [this, skillPtr, isCore]() {
					std::vector<ContextMenu::Item> ret{};
					const auto &promotion = Misc::promotions.at(agent->state.stats.sheet.promotion);
					for (const auto &skillLvl: std::views::iota(static_cast<uint8_t>(0), isCore ? promotion.maxCore + 1u : promotion.maxSkill)) {
						ret.emplace_back(ContextMenu::Button{
							.text = fmt::format("Lvl {}", isCore ? skillLvl : skillLvl + 1),
							.callback = [this, skillLvl, skillPtr]() {
								setState([&]() {
									std::invoke(skillPtr, agent->state.stats.sheet.skills).constant = skillLvl;
								});
							},
						});
					}
					return ret;
				}(),
			},
		};
	};

	Child content = Column{
		.spacing = 16.f,
		.children{
			levelPromotionSelector,
			mindscapeSelector,
			getSkillSelector("Basic", &SkillType::basic),
			getSkillSelector("Dodge", &SkillType::dodge),
			getSkillSelector("Assist", &SkillType::assist),
			getSkillSelector("Special", &SkillType::special),
			getSkillSelector("Chain", &SkillType::chain),
			getSkillSelector("Core", &SkillType::core, true),
		},
	};

	Children buttonFooter{
		Button{
			.widget{.width = Size::Expand},
			.theme = Button::Theme::Accent(element),
			.onClick = [this]() {
				closeEvent.notify();
				if (widget->onSubmit) widget->onSubmit(*agent);
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
		.title = "Edit agent",
		.content = content,
		.buttons = buttonFooter,
	};
}
