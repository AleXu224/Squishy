#pragma once

#include "artifact/instance.hpp"
#include "artifact/key.hpp"
#include "artifact/slot.hpp"


namespace Optimization {
	struct UpgradeFilter {
		::Artifact::Slot slot;
		Artifact::SetKey set = 0;

		std::vector<Artifact::Instance> filter(const std::vector<Artifact::Instance> &artifacts) const {
			std::vector<Artifact::Instance> ret{};
			for (const auto &artifact: artifacts) {
				if (artifact.slot != slot) continue;
				if (set && artifact.set != set) continue;
				ret.emplace_back(artifact);
			}
			return ret;
		}
	};
}// namespace Optimization