#pragma once

#include "stats/sheet.hpp"

namespace Modifiers::Enemy {
	// static constexpr Modifiers::EnemyFactory<InstanceStats, EnemyPointerFactory<Stats::EnemySheet<Stats::Value<float, 0>>>{}, EnemyNameFactory{}> instanceStats{};
	const Stats::EnemySheet<Formula::FloatNode> &instanceStats();
}// namespace Modifiers::Enemy