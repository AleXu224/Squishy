#pragma once

#include "stats/sheet.hpp"

namespace Modifiers::Engine {
	// static constexpr Modifiers::StatFactory<InstanceStats, StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{}> instanceStats{};
	const Stats::Sheet<Formula::FloatNode> &instanceBase();
	const Stats::Sheet<Formula::FloatNode> &instanceInitial();
}// namespace Modifiers::Engine