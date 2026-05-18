#pragma once

#include "disc/key.hpp"

namespace Optimization {
	struct SolutionUpgrade {
		float upgradeChance;
		float upgradeAverage;
		Disc::InstanceKey disc;
	};

	struct SolutionsUpgrade {
		static inline std::mutex mtx;
		std::array<SolutionUpgrade, 5> solutions{};
		float maxUpgradeChance = 0.f;
		float minUpgradeChance = 0.f;

		void addSolution(const SolutionUpgrade &solution) {
			std::scoped_lock _{mtx};
			if (solution.upgradeChance <= minUpgradeChance) return;

			for (auto &s: solutions) {
				if (s.upgradeChance < solution.upgradeChance) std::swap(s, solutions.back());
			}
			for (auto &s: solutions) {
				if (s.upgradeChance < solution.upgradeChance) {
					s = solution;
					break;
				}
			}
			maxUpgradeChance = solutions.front().upgradeChance;
			minUpgradeChance = solutions.back().upgradeChance;
		}
	};
}// namespace Optimization