#pragma once

#include "stats/sheet.hpp"

namespace Modifiers::Engine {
	// static constexpr StatFactory<Frm, Engine::Passive::preMods, Engine::Passive::postMods, Engine::instanceStats> displayStats;
	const Stats::Sheet<Formula::FloatNode> &displayStats();
}// namespace Modifiers::Engine