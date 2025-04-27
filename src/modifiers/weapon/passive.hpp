#pragma once

#include "modifiers/helpers.hpp"// IWYU pragma: keep
#include "stats/sheet.hpp"


namespace Modifiers::Weapon::Passive {
	// static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::preMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> preMods;
	// static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::postMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> postMods;
	// static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::teamPreMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> teamPreMods;
	// static constexpr StatFactory<FormulaMaker<&Stats::ModsSheet::teamPostMod, SheetMember<Stats::ModsSheet::_Sheet>>::Frm, StatPointerFactory<Stats::ModsSheet::_Sheet>{}, StatNameFactory{}> teamPostMods;
	// static constexpr TalentFactory<FormulaMaker<&Stats::ModsSheet::talents, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}, TalentNameFactory{}> talents;
	// static constexpr TalentFactory<FormulaMaker<&Stats::ModsSheet::teamTalents, TalentMember<Stats::ModsSheet::_Talents>>::Frm, TalentPointerFactory<Stats::ModsSheet::_Talents>{}, TalentNameFactory{}> teamTalents;
	// static constexpr EnemyFactory<FormulaMaker<&Stats::ModsSheet::enemy, SheetMember<Stats::ModsSheet::_EnemySheet>>::Frm, EnemyPointerFactory<Stats::ModsSheet::_EnemySheet>{}, EnemyNameFactory{}> enemy;
	const Stats::Sheet<Formula::FloatNode> &preMods();
	const Stats::Sheet<Formula::FloatNode> &postMods();
	const Stats::Sheet<Formula::FloatNode> &teamPreMods();
	const Stats::Sheet<Formula::FloatNode> &teamPostMods();
	const Talents<Formula::IntNode> &talents();
	const Talents<Formula::IntNode> &teamTalents();
	const Stats::EnemySheet<Formula::FloatNode> &enemy();
}// namespace Modifiers::Weapon::Passive