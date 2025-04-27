#pragma once

#include "stats/sheet.hpp"


namespace Modifiers::Character {
	// static constexpr Modifiers::StatFactory<InstanceStats, StatPointerFactory<Stats::Sheet<Stats::Value<float, 2>>>{}, StatNameFactory{}> instanceStats{};
	// static constexpr Modifiers::TalentFactory<InstanceTalents, TalentPointerFactory<Talents<Stats::Value<int32_t, 1>>>{}> instanceTalents{};
	const Stats::Sheet<Formula::FloatNode> &instanceStats();
	const Talents<Formula::IntNode> &instanceTalents();
}// namespace Modifiers::Character