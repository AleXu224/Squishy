#pragma once

#include "artifact/instance.hpp"
#include "character/instance.hpp"

namespace Optimization {
	struct BnbUpgrade {
		const Artifact::Instance &artifact;
		Character::Instance &character;
		const Formula::Context &ctx;
		const Formula::FloatNode &node;
		uint32_t slot;
		float currentScore;

		struct Agg {
			uint64_t successfulRolls = 0;
			double upgradeScores = 0.f;
		};

		std::unordered_map<uint64_t, Agg> memo;

		struct State {
			std::array<std::array<uint8_t, 4>, 4> count{};

			uint64_t hash() const {
				uint64_t ret = 0;
				for (const auto &statCount: count) {
					for (const auto &count: statCount) {
						ret <<= 4;
						ret |= count;
					}
				}
				return ret;
			}

			Stats::Sheet<float> getSheet(const Artifact::Instance &artifact) const {
				Stats::Sheet<float> sheet = artifact.stats;
				for (size_t i = 0; i < count.size(); i++) {
					auto stat = artifact.subStats.at(i).stat;
					if (!stat.has_value()) continue;
					for (size_t j = 0; j < count[i].size(); j++) {
						auto rollCount = this->count[i][j];
						if (rollCount == 0) continue;
						sheet.fromStat(stat.value()) += Stats::Values::subStat.at(stat.value()).star5.at(j) * static_cast<float>(rollCount);
					}
				}
				return sheet;
			}

			Stats::Sheet<float> getMaxPotential(const Artifact::Instance &artifact, uint32_t rollsLeft) const {
				Stats::Sheet<float> sheet = artifact.stats;
				for (size_t i = 0; i < count.size(); i++) {
					auto stat = artifact.subStats.at(i).stat;
					if (!stat.has_value()) continue;
					auto &sheetVal = sheet.fromStat(stat.value());
					for (size_t j = 0; j < count[i].size(); j++) {
						auto rollCount = this->count[i][j];
						if (rollCount == 0) continue;
						sheetVal += Stats::Values::subStat.at(stat.value()).star5.at(j) * static_cast<float>(rollCount);
					}
					sheetVal += Stats::Values::subStat.at(stat.value()).star5.at(3) * static_cast<float>(rollsLeft);
				}
				return sheet;
			}
		};

		inline Agg solve(State state, uint8_t rollsLeft) {
			if (auto it = memo.find(state.hash()); it != memo.end()) {
				return it->second;
			}

			auto &loadout = character.state.loadout();
			if (rollsLeft == 0) {
				auto sheet = state.getSheet(artifact);
				loadout.artifact.sheet.equippedArtifacts.at(slot) = &sheet;
				auto score = node.eval(ctx);
				if (score <= currentScore) {
					return Agg{};
				}
				return Agg{
					.successfulRolls = 1,
					.upgradeScores = score - currentScore,
				};
			}

			auto maxPotential = state.getMaxPotential(artifact, rollsLeft);
			loadout.artifact.sheet.equippedArtifacts.at(slot) = &maxPotential;
			auto score = node.eval(ctx);
			if (score <= currentScore) {
				return Agg{};
			}

			Agg agg{};
			for (size_t i = 0; i < state.count.size(); i++) {
				auto stat = artifact.subStats.at(i).stat;
				if (!stat.has_value()) continue;
				for (size_t j = 0; j < state.count[i].size(); j++) {
					auto next = state;
					next.count[i][j]++;
					auto subAgg = solve(next, rollsLeft - 1);
					agg.successfulRolls += subAgg.successfulRolls;
					agg.upgradeScores += subAgg.upgradeScores;
				}
			}

			memo.emplace(state.hash(), agg);
			return agg;
		}
	};
}// namespace Optimization