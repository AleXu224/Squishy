#pragma once

#include "stats/sheet.hpp"

namespace Modifiers::Weapon {
	// static constexpr Modifiers::StatFactory<InstanceStats, StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{}> instanceStats{};
	const Stats::Sheet<Formula::FloatNode> &instanceStats();
}// namespace Modifiers::Weapon