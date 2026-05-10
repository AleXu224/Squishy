#pragma once

#include "combo/source.hpp"
#include "disc/key.hpp"
#include "disc/sets.hpp"
#include "optional"
#include "stats/disc.hpp"
#include "unordered_map"
#include <map>


namespace Optimization {
	struct Options {
		std::optional<Combo::Source::Types> nodeSource;
		std::unordered_map<Disc::SetKey, bool> twoPcSets{};
		std::unordered_map<Disc::SetKey, bool> fourPcSets{};
		std::map<Stat, bool> partition4MainStats{};
		std::map<Stat, bool> partition5MainStats{};
		std::map<Stat, bool> partition6MainStats{};
		bool useEquippedDiscs = true;

		Options() {
			for (const auto &[key, set]: Disc::sets) {
				twoPcSets[key] = true;
				fourPcSets[key] = true;
			}

			for (const auto &mainStat: Stats::Disc::partition4) {
				partition4MainStats[mainStat] = true;
			}
			for (const auto &mainStat: Stats::Disc::partition5) {
				partition5MainStats[mainStat] = true;
			}
			for (const auto &mainStat: Stats::Disc::partition6) {
				partition6MainStats[mainStat] = true;
			}
		}

		auto makeEnabledSets() const {
			std::vector<Disc::SetKey> retTwoPcSets{};
			std::vector<Disc::SetKey> retFourPcSets{};

			for (const auto &[key, val]: twoPcSets) {
				if (val) retTwoPcSets.emplace_back(key);
			}
			for (const auto &[key, val]: fourPcSets) {
				if (val) retFourPcSets.emplace_back(key);
			}

			return std::pair{retTwoPcSets, retFourPcSets};
		}

		void removeComboIfSelected(const Combo::InstanceKey &comboKey) {
			if (nodeSource) {
				if (auto comboSource = std::get_if<Combo::Source::Combo>(&nodeSource.value())) {
					if (comboSource->comboKey == comboKey) {
						nodeSource.reset();
					}
				}
			}
		}
	};
}// namespace Optimization