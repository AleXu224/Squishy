#pragma once

#include "combo.hpp"
#include "key.hpp"// IWYU pragma: keep
#include "optimization/options.hpp"


namespace Serialization::Save {
	struct Optimization {
		std::optional<ComboSourceTypes> nodeSource;
		std::unordered_map<::Artifact::SetKey, bool> twoPcSets{};
		std::unordered_map<::Artifact::SetKey, bool> fourPcSets{};
		bool threeRainbow = true;
		bool fiveRainbow = true;

		static Optimization fromInstance(const ::Optimization::Options &);
		::Optimization::Options toInstance() const;
	};
}// namespace Serialization::Save