#pragma once

#include "stats/disc.hpp"


namespace Optimization {
	struct SolutionTC {
		Stats::Disc::Theorycraft discs;
		float score;
	};
}// namespace Optimization