#pragma once

#include "stats/sheet.hpp"

namespace Modifiers {
	// static constexpr StatFactory<PreModFrm, Agent::Kit::preMods, Agent::instanceStats, Engine::Passive::preMods, Engine::instanceStats, Disc::Set::preMods, Disc::subStats, Team::preMods, Team::resonances> preMods;
	const Stats::Sheet<Formula::FloatNode> &initial();
}// namespace Modifiers