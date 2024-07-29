#pragma once

#include "artifact/sets/GildedDreams.hpp"
#include "set.hpp"

#include "unordered_map"

namespace Artifact {
	inline std::unordered_map<Artifact::SetKey, const Artifact::Set &> sets;

	inline void initArtifacts() {
		sets = {
			{Artifact::Sets::gildedDreams.key, Artifact::Sets::gildedDreams},
		};
	}
}// namespace Artifact
