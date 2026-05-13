#include "decodeModsSheet.hpp"

#include "UI/utils/skillEntry.hpp"
#include "misc/attribute.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/skillFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/helpers.hpp"
#include "widgets/tooltip.hpp"


using namespace squi;

namespace {
	void addItem(auto &&stat, Modifiers::SheetMemberIdentifier identifier, Children &ret, const Formula::Context &ctx, bool &transparent, std::string_view prefix) {
		if (!stat.hasValue()) return;
		auto message = stat.print(ctx);
		auto value = stat.eval(ctx);

		if (value == 0) return;
		ret.emplace_back(Tooltip{
			.text = std::move(message),
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
		for (const auto &attribute: Misc::damageAttributes) {
			for (const auto &skill: Misc::skillStats) {
				addItem(Stats::fromDamageAttribute(sheet, attribute, skill), Stats::fromDamageAttribute<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(attribute, skill), ret, ctx, transparent, prefix);
			}
		}
		for (const auto &attackSource: Misc::attackSources) {
			for (const auto &skill: Misc::skillStats) {
				addItem(Stats::fromAttackSource(sheet, attackSource, skill), Stats::fromAttackSource<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(attackSource, skill), ret, ctx, transparent, prefix);
			}
		}
		for (const auto &anomaly: Misc::damageAnomalies) {
			for (const auto &skill: Misc::skillStats) {
				addItem(Stats::fromAnomaly(sheet, anomaly, skill), Stats::fromAnomaly<Modifiers::StatNameFactory{}, Modifiers::SheetMemberIdentifier>(anomaly, skill), ret, ctx, transparent, prefix);
			}
		}

		return ret;
	}

	[[nodiscard]] Children decodeEnemySheet(std::string_view prefix, bool &transparent, const Stats::ModsSheet::_EnemySheet &sheet, const Formula::Context &ctx) {
		Children ret;

		addItem(sheet.level, Modifiers::EnemyNameFactory::level, ret, ctx, transparent, prefix);
		addItem(sheet.stunMod, Modifiers::EnemyNameFactory::stunMod, ret, ctx, transparent, prefix);
		addItem(sheet.baseDef, Modifiers::EnemyNameFactory::baseDef, ret, ctx, transparent, prefix);
		addItem(sheet.DEFReduction, Modifiers::EnemyNameFactory::DEFReduction, ret, ctx, transparent, prefix);
		addItem(sheet.DEFIgnored, Modifiers::EnemyNameFactory::DEFIgnored, ret, ctx, transparent, prefix);
		addItem(sheet.dazeRes, Modifiers::EnemyNameFactory::dazeRes, ret, ctx, transparent, prefix);
		addItem(sheet.daze, Modifiers::EnemyNameFactory::daze, ret, ctx, transparent, prefix);
		addItem(sheet.dazeTaken, Modifiers::EnemyNameFactory::dazeTaken, ret, ctx, transparent, prefix);
		for (const auto &attribute: Misc::attributes) {
			addItem(sheet.resistance.fromAttribute(attribute), Stats::fromEnemyResAttribute<Modifiers::EnemyNameFactory{}.resistance, Modifiers::SheetMemberIdentifier>(attribute), ret, ctx, transparent, prefix);
		}

		return ret;
	}

	[[nodiscard]] Children decodeSkillsSheet(std::string_view prefix, bool &transparent, const Stats::ModsSheet::_Skills &sheet, const Formula::Context &ctx) {
		Children ret;

		addItem(sheet.basic, Modifiers::SkillNameFactory::basic, ret, ctx, transparent, prefix);
		addItem(sheet.dodge, Modifiers::SkillNameFactory::dodge, ret, ctx, transparent, prefix);
		addItem(sheet.assist, Modifiers::SkillNameFactory::assist, ret, ctx, transparent, prefix);
		addItem(sheet.special, Modifiers::SkillNameFactory::special, ret, ctx, transparent, prefix);
		addItem(sheet.chain, Modifiers::SkillNameFactory::chain, ret, ctx, transparent, prefix);
		addItem(sheet.core, Modifiers::SkillNameFactory::core, ret, ctx, transparent, prefix);

		return ret;
	}
}// namespace

squi::Children UI::decodeModsSheet(const Stats::ModsSheet &sheet, const Formula::Context &ctx, bool *transparent) {
	Children ret{};

	bool transparentBackup;
	if (!transparent) transparent = &transparentBackup;

	auto base = decodeSheet("Base ", *transparent, sheet.base, ctx);
	ret.insert(ret.end(), base.begin(), base.end());
	auto initial = decodeSheet("Initial ", *transparent, sheet.initial, ctx);
	ret.insert(ret.end(), initial.begin(), initial.end());
	auto combat = decodeSheet("Combat ", *transparent, sheet.combat, ctx);
	ret.insert(ret.end(), combat.begin(), combat.end());
	auto teamInitial = decodeSheet("Team Initial ", *transparent, sheet.teamInitial, ctx);
	ret.insert(ret.end(), teamInitial.begin(), teamInitial.end());
	auto teamCombat = decodeSheet("Team Combat ", *transparent, sheet.teamCombat, ctx);
	ret.insert(ret.end(), teamCombat.begin(), teamCombat.end());
	auto activeInitial = decodeSheet("Active Initial ", *transparent, sheet.activeInitial, ctx);
	ret.insert(ret.end(), activeInitial.begin(), activeInitial.end());
	auto activeCombat = decodeSheet("Active Combat ", *transparent, sheet.activeCombat, ctx);
	ret.insert(ret.end(), activeCombat.begin(), activeCombat.end());
	auto enemy = decodeEnemySheet("Enemy ", *transparent, sheet.enemy, ctx);
	ret.insert(ret.end(), enemy.begin(), enemy.end());
	auto skills = decodeSkillsSheet("", *transparent, sheet.skills, ctx);
	ret.insert(ret.end(), skills.begin(), skills.end());
	auto teamSkills = decodeSkillsSheet("Team ", *transparent, sheet.teamSkills, ctx);
	ret.insert(ret.end(), teamSkills.begin(), teamSkills.end());

	return ret;
}

squi::Children UI::decodeOption(const Option::Types &option, const Formula::Context &ctx) {
	Children ret;

	bool transparent;

	std::visit(
		[&](auto &&opt) {
			auto base = decodeSheet("Base ", transparent, opt.mods.base, ctx);
			ret.insert(ret.end(), base.begin(), base.end());
			auto initial = decodeSheet("Initial ", transparent, opt.mods.initial, ctx);
			ret.insert(ret.end(), initial.begin(), initial.end());
			auto combat = decodeSheet("Combat ", transparent, opt.mods.combat, ctx);
			ret.insert(ret.end(), combat.begin(), combat.end());
			auto teamInitial = decodeSheet("Team Initial ", transparent, opt.mods.teamInitial, ctx);
			ret.insert(ret.end(), teamInitial.begin(), teamInitial.end());
			auto teamCombat = decodeSheet("Team Combat ", transparent, opt.mods.teamCombat, ctx);
			ret.insert(ret.end(), teamCombat.begin(), teamCombat.end());
			auto activeInitial = decodeSheet("Active Initial ", transparent, opt.mods.activeInitial, ctx);
			ret.insert(ret.end(), activeInitial.begin(), activeInitial.end());
			auto activeCombat = decodeSheet("Active Combat ", transparent, opt.mods.activeCombat, ctx);
			ret.insert(ret.end(), activeCombat.begin(), activeCombat.end());
			auto enemy = decodeEnemySheet("Enemy ", transparent, opt.mods.enemy, ctx);
			ret.insert(ret.end(), enemy.begin(), enemy.end());
			auto skills = decodeSkillsSheet("", transparent, opt.mods.skills, ctx);
			ret.insert(ret.end(), skills.begin(), skills.end());
			auto teamSkills = decodeSkillsSheet("Team ", transparent, opt.mods.teamSkills, ctx);
			ret.insert(ret.end(), teamSkills.begin(), teamSkills.end());

			for (const Node::Types &node: opt.nodes) {
				auto value = node.formula.eval(ctx);
				if (value == 0) continue;
				ret.emplace_back(UI::Tooltip{
					.text = node.formula.print(ctx),
					.child = UI::SkillEntry{
						.isTransparent = transparent = !transparent,
						.name = std::string(node.name),
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
