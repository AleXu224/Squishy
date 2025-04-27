#pragma once

#include "stats/sheet.hpp"


namespace Modifiers::Artifact {
	// static constexpr Modifiers::StatFactory<SubStatFormula, StatPointerFactory<Stats::Sheet<float>>{}, StatNameFactory{}> subStats{};
	const Stats::Sheet<Formula::FloatNode> &subStats();
}// namespace Modifiers::Artifact