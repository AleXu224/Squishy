#include "characterSheet.hpp"

#include "formula/formula.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

void Stats::CharacterSheet::init(Stats::Loadout &stats) {
	using namespace Formula::Operators;
	// HP
	this->stats.baseHp.modifiers.at(0) = Formula::Prefix(
		"Character Base",
		Formula::Custom([](const Formula::Context &context) {
			return context.source.character.base.getHpAt(context.source.character.sheet.level, context.source.character.sheet.ascension);
		})
	);
	this->stats.hp.modifiers.at(0) = (Modifiers::total.hp_ + 1.f) * Modifiers::total.baseHp;

	// ATK
	this->stats.baseAtk.modifiers.at(0) = Formula::Prefix(
		"Character Base",
		Formula::Custom([](const Formula::Context &context) {
			return context.source.character.base.getAtkAt(context.source.character.sheet.level, context.source.character.sheet.ascension);
		})
	);
	this->stats.atk.modifiers.at(0) = (Modifiers::total.atk_ + 1.f) * Modifiers::total.baseAtk;

	// DEF
	this->stats.baseDef.modifiers.at(0) = Formula::Prefix(
		"Character Base",
		Formula::Custom([](const Formula::Context &context) {
			return context.source.character.base.getDefAt(context.source.character.sheet.level, context.source.character.sheet.ascension);
		})
	);
	this->stats.def.modifiers.at(0) = (Modifiers::total.def_ + 1.f) * Modifiers::total.baseDef;

	// Ascension stat
	this->stats.fromStat(stats.character.base.ascensionStat).modifiers.at(0) = Formula::Prefix(
		"Character Base",
		Formula::Custom(
			[](const Formula::Context &context) {
				return context.source.character.base.getAscensionStatAt(context.source.character.sheet.ascension);
			},
			Utils::isPercentage(stats.character.base.ascensionStat)
		)
	);

	// Constellation talents
	this->talents.fromTalent(stats.character.base.c3Talent).modifiers.at(0) = Formula::Requires(Requirement::constellation3, Formula::ConstantInt(3));
	this->talents.fromTalent(stats.character.base.c5Talent).modifiers.at(0) = Formula::Requires(Requirement::constellation5, Formula::ConstantInt(3));
}
