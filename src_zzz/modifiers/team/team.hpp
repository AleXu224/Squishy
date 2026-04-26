#pragma once

#include "formula/operators.hpp"// IWYU pragma: keep
#include "stats/sheet.hpp"

namespace Modifiers::Team {
	const Stats::Sheet<Formula::FloatNode> &initial();
	const Stats::Sheet<Formula::FloatNode> &combat();
	const Stats::Sheet<Formula::FloatNode> &activeInitial();
	const Stats::Sheet<Formula::FloatNode> &activeCombat();
	const Skills<Formula::IntNode> &skills();
	const Stats::EnemySheet<Formula::FloatNode> &enemy();
}// namespace Modifiers::Team