#pragma once

#include "formula/operators.hpp"// IWYU pragma: keep
#include "stats/sheet.hpp"

namespace Modifiers::Team {
	// static constexpr Modifiers::StatFactory<ResonanceFrm, StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{}> resonances;
	// static constexpr EnemyFactory<EnemyResonanceFrm, EnemyPointerFactory<Stats::EnemySheet<Stats::Value<float, 1>>>{}, EnemyNameFactory{}> resonancesEnemy;
	const Stats::Sheet<Formula::FloatNode> &resonances();
	const Stats::EnemySheet<Formula::FloatNode> &resonancesEnemy();
}// namespace Modifiers::Team