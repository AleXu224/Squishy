#pragma once

#include "agent/instance.hpp"
#include "disc/instance.hpp"


namespace Optimization {
	struct BnbUpgrade {
		const Disc::Instance &disc;
		Agent::Instance &agent;
		const Formula::Context &ctx;
		const Formula::FloatNode &node;
		uint32_t partition;
		float currentScore;

		struct Agg {
			uint64_t successfulRolls = 0;
			double upgradeScores = 0.f;
		};

		std::unordered_map<uint64_t, Agg> memo;

		struct State {
			std::array<uint8_t, 4> count{};

			uint64_t hash() const {
				uint64_t ret = 0;
				for (const auto &statCount: count) {
					ret <<= 4;
					ret |= statCount;
				}
				return ret;
			}

			Stats::Sheet<float> getSheet(const Disc::Instance &disc) const {
				Stats::Sheet<float> sheet = disc.stats;
				for (size_t i = 0; i < count.size(); i++) {
					auto stat = disc.subStats.at(i).stat;
					if (!stat.has_value()) continue;
					auto rollCount = this->count[i];
					if (rollCount == 0) continue;
					sheet.fromStat(stat.value()) += Stats::Values::subStat.at(stat.value()).fiveStar * static_cast<float>(rollCount);
				}
				return sheet;
			}

			Stats::Sheet<float> getMaxPotential(const Disc::Instance &disc, uint32_t rollsLeft) const {
				Stats::Sheet<float> sheet = disc.stats;
				for (size_t i = 0; i < count.size(); i++) {
					auto stat = disc.subStats.at(i).stat;
					if (!stat.has_value()) continue;
					auto &sheetVal = sheet.fromStat(stat.value());
					auto rollCount = this->count[i];
					if (rollCount == 0) continue;
					sheetVal += Stats::Values::subStat.at(stat.value()).fiveStar * static_cast<float>(rollCount + rollsLeft);
				}
				return sheet;
			}
		};

		inline Agg solve(State state, uint8_t rollsLeft) {
			if (auto it = memo.find(state.hash()); it != memo.end()) {
				return it->second;
			}

			auto &loadout = agent.state.loadout();
			if (rollsLeft == 0) {
				auto sheet = state.getSheet(disc);
				loadout.disc.sheet.equippedDiscs.at(partition) = &sheet;
				auto score = node.eval(ctx);
				if (score <= currentScore) {
					return Agg{};
				}
				return Agg{
					.successfulRolls = 1,
					.upgradeScores = score - currentScore,
				};
			}

			auto maxPotential = state.getMaxPotential(disc, rollsLeft);
			loadout.disc.sheet.equippedDiscs.at(partition) = &maxPotential;
			auto score = node.eval(ctx);
			if (score <= currentScore) {
				return Agg{};
			}

			Agg agg{};
			for (size_t i = 0; i < state.count.size(); i++) {
				auto stat = disc.subStats.at(i).stat;
				if (!stat.has_value()) continue;
				auto next = state;
				next.count[i]++;
				auto subAgg = solve(next, rollsLeft - 1);
				agg.successfulRolls += subAgg.successfulRolls;
				agg.upgradeScores += subAgg.upgradeScores;
			}

			memo.emplace(state.hash(), agg);
			return agg;
		}
	};
}// namespace Optimization