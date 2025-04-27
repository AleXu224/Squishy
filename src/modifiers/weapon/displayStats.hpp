#pragma once

#include "stats/sheet.hpp"

namespace Modifiers::Weapon {
	// static constexpr StatFactory<Frm, Weapon::Passive::preMods, Weapon::Passive::postMods, Weapon::instanceStats> displayStats;
	const Stats::Sheet<Formula::FloatNode> &displayStats();
}// namespace Modifiers::Weapon