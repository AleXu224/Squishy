#pragma once

#include "stats/artifact.hpp"


namespace Optimization {
	struct SolutionTC {
		Stats::Artifact::Theorycraft artifacts;
		float score;
	};
}// namespace Optimization