#pragma once

#include "disc/key.hpp"

namespace Optimization {
	struct SolutionUpgrade {
		float upgradeChance;
		float upgradeAverage;
		float score;
		Disc::InstanceKey disc;
	};

	struct SolutionsUpgrade {
		static inline std::mutex mtx;
		std::array<SolutionUpgrade, 20> solutions{};
		float maxScore = 0.f;
		float minScore = 0.f;

		void addSolution(const SolutionUpgrade &solution) {
			std::scoped_lock _{mtx};
			if (solution.score <= minScore) return;

			for (auto &s: solutions) {
				if (s.score < solution.score) std::swap(s, solutions.back());
			}
			for (auto &s: solutions) {
				if (s.score < solution.score) {
					s = solution;
					break;
				}
			}
			maxScore = solutions.front().score;
			minScore = solutions.back().score;
		}
	};
}// namespace Optimization