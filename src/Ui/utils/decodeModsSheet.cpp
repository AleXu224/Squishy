#include "decodeModsSheet.hpp"

#include "Ui/elementToColor.hpp"
#include "misc/element.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "skillEntry.hpp"
#include "stats/helpers.hpp"
#include "tooltip.hpp"


using namespace squi;

namespace {
	void addItem(auto &&stat, Modifiers::SheetMemberIdentifier identifier, Children &ret, const Formula::Context &ctx, bool &transparent, std::string_view prefix) {
		if (!stat.hasValue()) return;
		auto message = stat.print(ctx);
		auto value = stat.eval(ctx);

		if (value == 0) return;
		ret.emplace_back(UI::Tooltip{
			.message = message,
			.child = UI::SkillEntry{
				.isTransparent = transparent = !transparent,
				.name = std::format("{}{}", prefix, identifier.getName()),
				.value = static_cast<float>(value),
				.color = squi::Color(0xFFFFFFFF),
				.isPercentage = identifier.isPercentage(),
			},
		});
	}

	[[nodiscard]] Children decodeSheet(std::string_view prefix, bool &transparent, const Stats::ModsSheet::_Sheet &sheet, const Formula::Context &ctx) {
		Children ret;

		for (const auto &stat: Stats::simple) {
			addItem(Stats::fromStat(sheet, stat), Stats::fromStat<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(stat), ret, ctx, transparent, prefix);
		}
		for (const auto &element: Misc::damageElements) {
			for (const auto &skill: Misc::skillStats) {
				addItem(Stats::fromDamageElement(sheet, element, skill), Stats::fromDamageElement<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(element, skill), ret, ctx, transparent, prefix);
			}
		}
		for (const auto &attackSource: Misc::attackSources) {
			for (const auto &skill: Misc::skillStats) {
				addItem(Stats::fromAttackSource(sheet, attackSource, skill), Stats::fromAttackSource<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(attackSource, skill), ret, ctx, transparent, prefix);
			}
		}
		for (const auto &reaction: Misc::reactions) {
			for (const auto &skill: Misc::skillStats) {
				addItem(Stats::fromReaction(sheet, reaction, skill), Stats::fromReaction<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(reaction, skill), ret, ctx, transparent, prefix);
			}
		}

		return ret;
	}

	[[nodiscard]] Children decodeEnemySheet(std::string_view prefix, bool &transparent, const Stats::ModsSheet::_EnemySheet &sheet, const Formula::Context &ctx) {
		Children ret;

		addItem(sheet.level, Modifiers::EnemyNameFactory::level, ret, ctx, transparent, prefix);
		addItem(sheet.DEFReduction, Modifiers::EnemyNameFactory::DEFReduction, ret, ctx, transparent, prefix);
		addItem(sheet.DEFIgnored, Modifiers::EnemyNameFactory::DEFIgnored, ret, ctx, transparent, prefix);
		for (const auto &element: Misc::elements) {
			addItem(sheet.resistance.fromElement(element), Stats::fromEnemyResElement<Modifiers::EnemyNameFactory{}.resistance, Modifiers::SheetMemberIdentifier>(element), ret, ctx, transparent, prefix);
		}

		return ret;
	}

	[[nodiscard]] Children decodeTalentsSheet(std::string_view prefix, bool &transparent, const Stats::ModsSheet::_Talents &sheet, const Formula::Context &ctx) {
		Children ret;

		addItem(sheet.normal, Modifiers::EnemyNameFactory::level, ret, ctx, transparent, prefix);
		addItem(sheet.skill, Modifiers::EnemyNameFactory::DEFReduction, ret, ctx, transparent, prefix);
		addItem(sheet.burst, Modifiers::EnemyNameFactory::DEFIgnored, ret, ctx, transparent, prefix);
		return ret;
	}

	[[nodiscard]] Children decodeInfusionSheet(std::string_view prefix, bool &transparent, const Formula::ElementNode &infusion, const Formula::Context &ctx) {
		Children ret;

		auto message = infusion.print(ctx);
		auto value = infusion.eval(ctx);
		if (!value.has_value()) return ret;
		ret.emplace_back(UI::Tooltip{
			.message = message,
			.child = UI::SkillEntry{
				.isTransparent = transparent = !transparent,
				.name = std::format("{}{} Infusion", prefix, message),
				.color = Utils::elementToColor(value.value()),
				.isPercentage = false,
			},
		});

		return ret;
	}
}// namespace

squi::Children UI::decodeModsSheet(Stats::ModsSheet &sheet, const Formula::Context &ctx) {
	Children ret{};

	bool transparent;

	auto preMod = decodeSheet("", transparent, sheet.preMod, ctx);
	ret.insert(ret.end(), preMod.begin(), preMod.end());
	auto postMod = decodeSheet("", transparent, sheet.postMod, ctx);
	ret.insert(ret.end(), postMod.begin(), postMod.end());
	auto teamPreMod = decodeSheet("Team ", transparent, sheet.teamPreMod, ctx);
	ret.insert(ret.end(), teamPreMod.begin(), teamPreMod.end());
	auto teamPostMod = decodeSheet("Team ", transparent, sheet.teamPostMod, ctx);
	ret.insert(ret.end(), teamPostMod.begin(), teamPostMod.end());
	auto enemy = decodeEnemySheet("Enemy ", transparent, sheet.enemy, ctx);
	ret.insert(ret.end(), enemy.begin(), enemy.end());
	auto talents = decodeTalentsSheet("", transparent, sheet.talents, ctx);
	ret.insert(ret.end(), talents.begin(), talents.end());
	auto teamTalents = decodeTalentsSheet("Team ", transparent, sheet.teamTalents, ctx);
	ret.insert(ret.end(), teamTalents.begin(), teamTalents.end());
	auto infusion = decodeInfusionSheet("", transparent, sheet.infusion, ctx);
	ret.insert(ret.end(), infusion.begin(), infusion.end());
	auto teamInfusion = decodeInfusionSheet("", transparent, sheet.teamInfusion, ctx);
	ret.insert(ret.end(), teamInfusion.begin(), teamInfusion.end());

	return ret;
}

squi::Children UI::decodeOption(const Option::Types &option, const Formula::Context &ctx) {
	Children ret;

	bool transparent;

	std::visit(
		[&](auto &&opt) {
			auto preMod = decodeSheet("", transparent, opt.mods.preMod, ctx);
			ret.insert(ret.end(), preMod.begin(), preMod.end());
			auto postMod = decodeSheet("", transparent, opt.mods.postMod, ctx);
			ret.insert(ret.end(), postMod.begin(), postMod.end());
			auto teamPreMod = decodeSheet("Team ", transparent, opt.mods.teamPreMod, ctx);
			ret.insert(ret.end(), teamPreMod.begin(), teamPreMod.end());
			auto teamPostMod = decodeSheet("Team ", transparent, opt.mods.teamPostMod, ctx);
			ret.insert(ret.end(), teamPostMod.begin(), teamPostMod.end());
			auto enemy = decodeEnemySheet("Enemy ", transparent, opt.mods.enemy, ctx);
			ret.insert(ret.end(), enemy.begin(), enemy.end());
			auto talents = decodeTalentsSheet("", transparent, opt.mods.talents, ctx);
			ret.insert(ret.end(), talents.begin(), talents.end());
			auto teamTalents = decodeTalentsSheet("Team ", transparent, opt.mods.teamTalents, ctx);
			ret.insert(ret.end(), teamTalents.begin(), teamTalents.end());
			auto infusion = decodeInfusionSheet("", transparent, opt.mods.infusion, ctx);
			ret.insert(ret.end(), infusion.begin(), infusion.end());
			auto teamInfusion = decodeInfusionSheet("", transparent, opt.mods.teamInfusion, ctx);
			ret.insert(ret.end(), teamInfusion.begin(), teamInfusion.end());

			for (const Node::Types &node: opt.nodes) {
				auto value = node.formula.eval(ctx);
				if (value == 0) continue;
				ret.emplace_back(UI::Tooltip{
					.message = node.formula.print(ctx),
					.child = UI::SkillEntry{
						.isTransparent = transparent = !transparent,
						.name = node.name,
						.value = value,
						.color = Node::getColor(node.data, ctx),
						.isPercentage = Node::isPercentage(node.data),
					},
				});
			}
		},
		option
	);

	return ret;
}
