#pragma once

#include "stats/sheet.hpp"

namespace Modifiers {
	// static constexpr StatFactory<TotalFrm, Character::Kit::postMods, Weapon::Passive::postMods, Artifact::Set::postMods, Team::postMods, preMods, StatNameFactory{}> total;
	// static constexpr StatFactory<TotalActiveFrm, Character::Kit::postMods, Weapon::Passive::postMods, Artifact::Set::postMods, Team::postMods, preMods, StatNameFactory{}> totalActive;
	// static constexpr StatFactory<DisplayTotalFrm, Character::Kit::postMods, Weapon::Passive::postMods, Artifact::Set::postMods, Team::postMods, preMods, StatNameFactory{}> displayTotal;
	// static constexpr TalentFactory<TotalTalentsFrm, Character::Kit::talents, Character::instanceTalents, Weapon::Passive::talents, Artifact::Set::talents, Team::talents> totalTalents;
	// static constexpr EnemyFactory<TotalEnemyFrm, Team::enemy, Team::resonancesEnemy, Enemy::instanceStats, EnemyNameFactory{}> totalEnemy;
	const Stats::Sheet<Formula::FloatNode> &total();
	const Stats::Sheet<Formula::FloatNode> &totalActive();
	const Stats::Sheet<Formula::FloatNode> &displayTotal();
	const Talents<Formula::IntNode> &totalTalents();
	const Stats::EnemySheet<Formula::FloatNode> &totalEnemy();
}// namespace Modifiers