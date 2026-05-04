#pragma once

#include "disc/key.hpp"
#include "disc/sets.hpp"
#include "combo/source.hpp"
#include "optional"
#include "unordered_map"


namespace Optimization {
	struct Options {
		std::optional<Combo::Source::Types> nodeSource;
		std::unordered_map<Disc::SetKey, bool> twoPcSets{};
		std::unordered_map<Disc::SetKey, bool> fourPcSets{};
		bool threeRainbow = true;
		bool fiveRainbow = true;
		bool useEquippedDiscs = true;

		Options() {
			for (const auto &[key, set]: Disc::sets) {
				twoPcSets[key] = true;
				fourPcSets[key] = true;
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