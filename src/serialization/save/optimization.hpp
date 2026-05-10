#pragma once

#include "combo.hpp"
#include "key.hpp"// IWYU pragma: keep
#include "optimization/options.hpp"


namespace Serialization::Save {
	struct Optimization {
		std::optional<ComboSourceTypes> nodeSource;
		std::unordered_map<::Artifact::SetKey, bool> twoPcSets{};
		std::unordered_map<::Artifact::SetKey, bool> fourPcSets{};
		std::map<Stat, bool> sandsMainStats{};
		std::map<Stat, bool> gobletMainStats{};
		std::map<Stat, bool> circletMainStats{};
		bool threeRainbow = true;
		bool fiveRainbow = true;
		bool useEquippedArtifacts = true;

		static Optimization fromInstance(const ::Optimization::Options &);
		::Optimization::Options toInstance() const;
	};
}// namespace Serialization::Save