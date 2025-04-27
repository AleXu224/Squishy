#pragma once

#include "formula/operators.hpp"// IWYU pragma: keep
#include "stats/sheet.hpp"

namespace Modifiers::Team {
	// static constexpr StatFactory<Frm, Character::Kit::teamPreMods, Weapon::Passive::teamPreMods, Artifact::Set::teamPreMods> preMods;
	// static constexpr StatFactory<Frm, Character::Kit::teamPostMods, Weapon::Passive::teamPostMods, Artifact::Set::teamPostMods> postMods;
	// static constexpr TalentFactory<Frm, Character::Kit::teamTalents, Weapon::Passive::teamTalents, Artifact::Set::teamTalents> talents;
	// static constexpr EnemyFactory<Frm, Character::Kit::enemy, Weapon::Passive::enemy, Artifact::Set::enemy> enemy;
	const Stats::Sheet<Formula::FloatNode> &preMods();
	const Stats::Sheet<Formula::FloatNode> &postMods();
	const Talents<Formula::IntNode> &talents();
	const Stats::EnemySheet<Formula::FloatNode> &enemy();
}// namespace Modifiers::Team