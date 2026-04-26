#pragma once

#include "stats/sheet.hpp"

namespace Modifiers {
	// static constexpr StatFactory<TotalFrm, Agent::Kit::postMods, Engine::Passive::postMods, Disc::Set::postMods, Team::postMods, preMods, StatNameFactory{}> total;
	// static constexpr StatFactory<TotalActiveFrm, Agent::Kit::postMods, Engine::Passive::postMods, Disc::Set::postMods, Team::postMods, preMods, StatNameFactory{}> totalActive;
	// static constexpr StatFactory<DisplayTotalFrm, Agent::Kit::postMods, Engine::Passive::postMods, Disc::Set::postMods, Team::postMods, preMods, StatNameFactory{}> displayTotal;
	// static constexpr SkillFactory<TotalSkillsFrm, Agent::Kit::skills, Agent::instanceSkills, Engine::Passive::skills, Disc::Set::skills, Team::skills> totalSkills;
	// static constexpr EnemyFactory<TotalEnemyFrm, Team::enemy, Team::resonancesEnemy, Enemy::instanceStats, EnemyNameFactory{}> totalEnemy;
	const Stats::Sheet<Formula::FloatNode> &combat();
	const Stats::Sheet<Formula::FloatNode> &team();
	const Skills<Formula::IntNode> &skills();
	const Stats::EnemySheet<Formula::FloatNode> &enemy();
}// namespace Modifiers