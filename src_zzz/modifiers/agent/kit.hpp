#pragma once

#include "modifiers/helpers.hpp"// IWYU pragma: keep
#include "stats/sheet.hpp"


namespace Modifiers::Agent::Kit {
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
}// namespace Modifiers::Agent::Kit