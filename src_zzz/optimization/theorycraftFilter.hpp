#pragma once

#include "stats/sheet.hpp"
#include "stats/stat.hpp"

#include <cstdint>
#include <functional>
#include <span>
namespace Optimization {
	struct RollsValues {
		float hp = Stats::Values::subStat.at(::Stat::hp).fiveStar;
		float hp_ = Stats::Values::subStat.at(::Stat::hp_).fiveStar;
		float atk = Stats::Values::subStat.at(::Stat::atk).fiveStar;
		float atk_ = Stats::Values::subStat.at(::Stat::atk_).fiveStar;
		float def = Stats::Values::subStat.at(::Stat::def).fiveStar;
		float def_ = Stats::Values::subStat.at(::Stat::def_).fiveStar;
		float ap = Stats::Values::subStat.at(::Stat::ap).fiveStar;
		float pen = Stats::Values::subStat.at(::Stat::pen).fiveStar;
		float cr = Stats::Values::subStat.at(::Stat::cr).fiveStar;
		float cd = Stats::Values::subStat.at(::Stat::cd).fiveStar;

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
				case ::Stat::ap:
					return self.ap;
				case ::Stat::pen:
					return self.pen;
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
		Stat ap{};
		Stat pen{};
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
				case ::Stat::ap:
					return self.ap;
				case ::Stat::pen:
					return self.pen;
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