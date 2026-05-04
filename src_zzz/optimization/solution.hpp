#pragma once

#include "mutex"
#include "stats/disc.hpp"


namespace Optimization {
	struct Solution {
		Stats::Disc::Slotted discs;
		float score;
	};

	struct Solutions {
		static inline std::mutex mtx;
		std::array<Solution, 5> solutions{};
		float maxScore = 0.f;
		float minScore = 0.f;

		void addSolution(const Stats::Disc::Slotted &discs, float score) {
			std::scoped_lock _{mtx};
			if (score <= minScore) return;

			for (auto &solution: solutions) {
				if (solution.score < score) std::swap(solution, solutions.back());
			}
			for (auto &solution: solutions) {
				if (solution.score < score) {
					solution = Solution{
						.discs = discs,
						.score = score,
					};
					break;
				}
			}
			maxScore = solutions.front().score;
			minScore = solutions.back().score;
		}
	};
}// namespace Optimization