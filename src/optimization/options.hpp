#pragma once

#include "artifact/key.hpp"
#include "artifact/sets.hpp"
#include "combo/source.hpp"
#include "optional"
#include "unordered_map"


namespace Optimization {
	struct Options {
		std::optional<Combo::Source::Types> nodeSource;
		std::unordered_map<Artifact::SetKey, bool> twoPcSets{};
		std::unordered_map<Artifact::SetKey, bool> fourPcSets{};
		bool threeRainbow = true;
		bool fiveRainbow = true;

		Options() {
			for (const auto &[key, set]: Artifact::sets) {
				twoPcSets[key] = true;
				fourPcSets[key] = true;
			}
		}

		auto makeEnabledSets() const {
			std::vector<Artifact::SetKey> retTwoPcSets{};
			std::vector<Artifact::SetKey> retFourPcSets{};

			for (const auto &[key, val]: twoPcSets) {
				if (val) retTwoPcSets.emplace_back(key);
			}
			for (const auto &[key, val]: fourPcSets) {
				if (val) retFourPcSets.emplace_back(key);
			}

			return std::pair{retTwoPcSets, retFourPcSets};
		}
	};
}// namespace Optimization