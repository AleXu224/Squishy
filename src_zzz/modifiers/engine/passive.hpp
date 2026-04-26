#pragma once

#include "modifiers/helpers.hpp"// IWYU pragma: keep
#include "stats/sheet.hpp"


namespace Modifiers::Engine::Passive {
	// static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::preMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> preMods;
	// static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::postMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> postMods;
	// static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::teamPreMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> teamPreMods;
	// static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::teamPostMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> teamPostMods;
	// static constexpr SkillFactory<FormulaMaker<&Stats::ModsSheet::skills, SkillMember<Stats::ModsSheet::_Skills>>::Frm, SkillPointerFactory<Stats::ModsSheet::_Skills>{}, SkillNameFactory{}> skills;
	// static constexpr SkillFactory<FormulaMaker<&Stats::ModsSheet::teamSkills, SkillMember<Stats::ModsSheet::_Skills>>::Frm, SkillPointerFactory<Stats::ModsSheet::_Skills>{}, SkillNameFactory{}> teamSkills;
	// static constexpr EnemyFactory<FormulaMaker<&Stats::ModsSheet::enemy, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm, EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{}, EnemyNameFactory{}> enemy;
	const Stats::Sheet<Formula::FloatNode> &base();
	const Stats::Sheet<Formula::FloatNode> &initial();
	const Stats::Sheet<Formula::FloatNode> &combat();
	const Stats::Sheet<Formula::FloatNode> &teamInitial();
	const Stats::Sheet<Formula::FloatNode> &teamCombat();
	const Stats::Sheet<Formula::FloatNode> &activeInitial();
	const Stats::Sheet<Formula::FloatNode> &activeCombat();
	const Skills<Formula::IntNode> &skills();
	const Skills<Formula::IntNode> &teamSkills();
	const Stats::EnemySheet<Formula::FloatNode> &enemy();
}// namespace Modifiers::Engine::Passive