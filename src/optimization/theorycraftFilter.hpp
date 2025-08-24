#pragma once

#include "stats/sheet.hpp"
#include "stats/stat.hpp"

#include <cstdint>
#include <functional>
#include <span>
namespace Optimization {
	struct RollsValues {
		float hp = Stats::Values::averageSubStat.at(::Stat::hp).at(4);
		float hp_ = Stats::Values::averageSubStat.at(::Stat::hp_).at(4);
		float atk = Stats::Values::averageSubStat.at(::Stat::atk).at(4);
		float atk_ = Stats::Values::averageSubStat.at(::Stat::atk_).at(4);
		float def = Stats::Values::averageSubStat.at(::Stat::def).at(4);
		float def_ = Stats::Values::averageSubStat.at(::Stat::def_).at(4);
		float er = Stats::Values::averageSubStat.at(::Stat::er).at(4);
		float em = Stats::Values::averageSubStat.at(::Stat::em).at(4);
		float cr = Stats::Values::averageSubStat.at(::Stat::cr).at(4);
		float cd = Stats::Values::averageSubStat.at(::Stat::cd).at(4);

		auto &&fromSubStat(this auto &&self, ::Stat stat) {
			switch (stat) {
				case ::Stat::hp:
					return self.hp;
				case ::Stat::hp_:
					return self.hp_;
				case ::Stat::atk:
					return self.atk;
				case ::Stat::atk_:
					return self.atk_;
				case ::Stat::def:
					return self.def;
				case ::Stat::def_:
					return self.def_;
				case ::Stat::er:
					return self.er;
				case ::Stat::em:
					return self.em;
				case ::Stat::cr:
					return self.cr;
				case ::Stat::cd:
					return self.cd;
				default:
					return self.hp;
			}
		}
	};

	struct TheorycraftFilter {
		struct Stat {
			uint8_t minRolls = 2;
			uint8_t maxRolls = 12;

			uint8_t getRange() const { return maxRolls - minRolls + 1; }
		};

		uint8_t maxRolls = 40;

		Stat hp{};
		Stat hp_{};
		Stat atk{};
		Stat atk_{};
		Stat def{};
		Stat def_{};
		Stat er{};
		Stat em{};
		Stat cr{};
		Stat cd{};

		auto &&fromSubStat(this auto &&self, ::Stat stat) {
			switch (stat) {
				case ::Stat::hp:
					return self.hp;
				case ::Stat::hp_:
					return self.hp_;
				case ::Stat::atk:
					return self.atk;
				case ::Stat::atk_:
					return self.atk_;
				case ::Stat::def:
					return self.def;
				case ::Stat::def_:
					return self.def_;
				case ::Stat::er:
					return self.er;
				case ::Stat::em:
					return self.em;
				case ::Stat::cr:
					return self.cr;
				case ::Stat::cd:
					return self.cd;
				default:
					return self.hp;
			}
		}

		bool hasTooManyRolls() const {
			uint8_t totalMinRolls = 0;
			for (const auto &stat: Stats::subStats) {
				totalMinRolls += fromSubStat(stat).minRolls;
			}
			return totalMinRolls > maxRolls;
		}

		void trimImpossibleMaxRolls() {
			uint8_t totalMinRolls = 0;
			for (const auto &stat: Stats::subStats) {
				auto &statFilter = fromSubStat(stat);
				totalMinRolls += statFilter.minRolls;
			}

			uint8_t remainingRolls = maxRolls - totalMinRolls;

			for (const auto &stat: Stats::subStats) {
				auto &statFilter = fromSubStat(stat);
				statFilter.maxRolls = statFilter.minRolls + std::min(static_cast<uint8_t>(statFilter.maxRolls - statFilter.minRolls), remainingRolls);
			}
		}

		uint64_t getCombCount() const {
			uint64_t count = 1;
			for (const auto &stat: Stats::subStats) {
				auto &statFilter = fromSubStat(stat);
				count *= static_cast<uint64_t>(statFilter.maxRolls - statFilter.minRolls + 1);
			}
			return count;
		}

		Stats::Sheet<float> getMaxStats(const RollsValues &rolls) const {
			Stats::Sheet<float> maxStats;
			for (const auto &stat: Stats::subStats) {
				maxStats.fromStat(stat) = static_cast<float>(fromSubStat(stat).maxRolls) * rolls.fromSubStat(stat);
			}
			return maxStats;
		}

		void iterateRolls(const std::function<void(uint8_t roll, ::Stat stat)> &applyStatFunc, const std::function<void()> &calcFunc, std::span<const ::Stat> statStack = Stats::subStats, uint8_t rollsUsed = 0) const {
			if (statStack.empty()) return;

			auto stat = statStack.front();
			auto &currentStat = fromSubStat(stat);
			for (uint8_t i = currentStat.minRolls; i <= currentStat.maxRolls; i++) {
				if (rollsUsed + i > maxRolls) break;
				applyStatFunc(i, stat);

				if (statStack.size() == 1) {
					calcFunc();
					continue;
				}

				iterateRolls(applyStatFunc, calcFunc, statStack.subspan(1), rollsUsed + i);
			}
		}
	};
}// namespace Optimization