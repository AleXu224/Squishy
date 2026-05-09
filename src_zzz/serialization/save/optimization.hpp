#pragma once

#include "combo.hpp"
#include "key.hpp"// IWYU pragma: keep
#include "optimization/options.hpp"


namespace Serialization::Save {
	struct Optimization {
		std::optional<ComboSourceTypes> nodeSource;
		std::unordered_map<::Disc::SetKey, bool> twoPcSets{};
		std::unordered_map<::Disc::SetKey, bool> fourPcSets{};

		static Optimization fromInstance(const ::Optimization::Options &);
		::Optimization::Options toInstance() const;
	};
}// namespace Serialization::Save