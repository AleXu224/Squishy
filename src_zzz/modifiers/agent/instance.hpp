#pragma once

#include "stats/sheet.hpp"


namespace Modifiers::Agent {
	// static constexpr Modifiers::StatFactory<InstanceStats, StatPointerFactory<Stats::Sheet<Stats::Value<float, 2>>>{}, StatNameFactory{}> instanceStats{};
	// static constexpr Modifiers::SkillFactory<InstanceSkills, SkillPointerFactory<Skills<Stats::Value<int32_t, 1>>>{}> instanceSkills{};
	const Stats::Sheet<Formula::FloatNode> &instanceBase();
	const Stats::Sheet<Formula::FloatNode> &instanceInitial();
	const Stats::Sheet<Formula::FloatNode> &instanceCombat();
	const Skills<Formula::IntNode> &instanceSkills();
}// namespace Modifiers::Agent