#pragma once

#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "stats/stat.hpp"

#include <array>
#include <mutex>

namespace Optimization {
	struct SolutionUpgrade {
		enum class Kind : uint8_t {
			levelUp,
			reroll,
			definition,
		};

		float upgradeChance = 0.f;
		float upgradeAverage = 0.f;
		float score = 0.f;
		Artifact::InstanceKey artifact{};

		Kind kind = Kind::levelUp;
		Artifact::SetKey set{};
		Artifact::Slot slot{};
		Stat mainStat{};
		std::array<Stat, 2> chosenSubStats{};
		uint8_t guaranteedRolls = 0;
	};

	struct SolutionsUpgrade {
		static inline std::mutex mtx;
		std::array<SolutionUpgrade, 5> solutions{};
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
