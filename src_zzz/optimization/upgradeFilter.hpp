#pragma once

#include "disc/instance.hpp"
#include "disc/key.hpp"
#include "disc/slot.hpp"


namespace Optimization {
	struct UpgradeFilter {
		::Disc::Partition partition;
		Disc::SetKey set = 0;

		std::vector<Disc::Instance> filter(const std::vector<Disc::Instance> &discs) const {
			std::vector<Disc::Instance> ret{};
			for (const auto &disc: discs) {
				if (disc.partition != partition) continue;
				if (set && disc.set != set) continue;
				ret.emplace_back(disc);
			}
			return ret;
		}
	};
}// namespace Optimization