#pragma once

#include "stats/sheet.hpp"

namespace Modifiers {
	// static constexpr StatFactory<PreModFrm, Character::Kit::preMods, Character::instanceStats, Weapon::Passive::preMods, Weapon::instanceStats, Artifact::Set::preMods, Artifact::subStats, Team::preMods, Team::resonances> preMods;
	const Stats::Sheet<Formula::FloatNode> &preMods();
}// namespace Modifiers