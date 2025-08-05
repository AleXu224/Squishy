#include "characterSheet.hpp"

#include "formula/formula.hpp"
#include "formula/teamCharacter.hpp"
#include "stats/loadout.hpp"


void Stats::CharacterSheet::init(Stats::State &stats) {
	using namespace Formula::Operators;
	// HP
	this->stats.baseHp.modifiers.at(0) = Formula::Prefix(
		"Character Base",
		Formula::Custom(
			[](const Formula::Context &context) -> Formula::Compiled::FloatNode {
				return Formula::Compiled::ConstantFloat{.value = context.source.stats.base.getHpAt(context.source.stats.sheet.level, context.source.stats.sheet.ascension)};
			},
			[](const Formula::Context &context) {
				return context.source.stats.base.getHpAt(context.source.stats.sheet.level, context.source.stats.sheet.ascension);
			}
		)
	);
	this->stats.hp.modifiers.at(0) = (Modifiers::total().hp_ + 1.f) * Modifiers::total().baseHp;

	// ATK
	this->stats.baseAtk.modifiers.at(0) = Formula::Prefix(
		"Character Base",
		Formula::Custom(
			[](const Formula::Context &context) -> Formula::Compiled::FloatNode {
				return Formula::Compiled::ConstantFloat{.value = context.source.stats.base.getAtkAt(context.source.stats.sheet.level, context.source.stats.sheet.ascension)};
			},
			[](const Formula::Context &context) {
				return context.source.stats.base.getAtkAt(context.source.stats.sheet.level, context.source.stats.sheet.ascension);
			}
		)
	);
	this->stats.atk.modifiers.at(0) = (Modifiers::total().atk_ + 1.f) * Modifiers::total().baseAtk;

	// DEF
	this->stats.baseDef.modifiers.at(0) = Formula::Prefix(
		"Character Base",
		Formula::Custom(
			[](const Formula::Context &context) -> Formula::Compiled::FloatNode {
				return Formula::Compiled::ConstantFloat{.value = context.source.stats.base.getDefAt(context.source.stats.sheet.level, context.source.stats.sheet.ascension)};
			},
			[](const Formula::Context &context) {
				return context.source.stats.base.getDefAt(context.source.stats.sheet.level, context.source.stats.sheet.ascension);
			}
		)
	);
	this->stats.def.modifiers.at(0) = (Modifiers::total().def_ + 1.f) * Modifiers::total().baseDef;

	// Ascension stat
	this->stats.fromStat(stats.stats.base.ascensionStat).modifiers.at(0) = Formula::Prefix(
		"Character Base",
		Formula::Custom(
			[](const Formula::Context &context) -> Formula::Compiled::FloatNode {
				return Formula::Compiled::ConstantFloat{.value = context.source.stats.base.getAscensionStatAt(context.source.stats.sheet.ascension)};
			},
			[](const Formula::Context &context) {
				return context.source.stats.base.getAscensionStatAt(context.source.stats.sheet.ascension);
			},
			Utils::isPercentage(stats.stats.base.ascensionStat)
		)
	);

	for (const auto &element: Misc::characterElements) {
		for (const auto &stat: Misc::skillStats) {
			this->stats.fromElement(element).fromSkillStat(stat).modifiers.at(1) = Formula::Prefix(
				"All Elemental",
				Formula::Custom(
					[stat](const Formula::Context &context) {
						return fromSkillStat(Modifiers::total().allElemental, stat).compile(context);
					},
					[stat](const Formula::Context &context) {
						return fromSkillStat(Modifiers::total().allElemental, stat).eval(context);
					},
					true
				)
			);
		}
	}

	// Constellation talents
	this->talents.fromTalent(stats.stats.base.c3Talent).modifiers.at(0) = Formula::Requires(Requirement::constellation3, Formula::ConstantInt(3));
	this->talents.fromTalent(stats.stats.base.c5Talent).modifiers.at(0) = Formula::Requires(Requirement::constellation5, Formula::ConstantInt(3));

	// Infusion
	infusion = Formula::CharacterTeamInfusion(stats.stats.data.data.mods.infusion);
	teamInfusion = stats.stats.data.data.mods.teamInfusion;

	// Moonsign
	moonsignLevel = stats.stats.data.data.mods.moonsignLevel;
}
