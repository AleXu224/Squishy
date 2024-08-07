#pragma once

#include "set.hpp"

#include "unordered_map"

namespace Artifact {
	inline std::unordered_map<Artifact::SetKey, const Artifact::Set &> sets;

	extern void initArtifacts();
}// namespace Artifact
