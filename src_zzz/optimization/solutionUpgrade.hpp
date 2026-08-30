#pragma once

#include "disc/key.hpp"
#include "disc/slot.hpp"
#include "stats/stat.hpp"

#include <array>
#include <mutex>

namespace Optimization {
	struct SolutionUpgrade {
		enum class Kind : uint8_t {
			levelUp,
			definition,
		};

		float upgradeChance = 0.f;
		float upgradeAverage = 0.f;
		float score = 0.f;
		Disc::InstanceKey disc{};

		Kind kind = Kind::levelUp;
		Disc::SetKey set{};
		Disc::Partition partition{};
		Stat mainStat{};
		std::array<Stat, 2> chosenSubStats{};
		uint8_t chosenCount = 0;
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
