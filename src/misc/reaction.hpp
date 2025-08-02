#pragma once

#include "cstdint"
#include "utils/stringify.hpp"
#include <utility>
#include <vector>


namespace Misc {
	enum class Reaction : uint8_t {
		vape,
		melt,

		aggravate,
		spread,

		burning,
		superconduct,
		swirl,
		electroCharged,
		shattered,
		overloaded,
		bloom,
		burgeon,
		hyperbloom,

		lunarCharged,
	};

	enum class TransformativeReaction : uint8_t {
		burning,
		superconduct,
		pyroSwirl,
		hydroSwirl,
		electroSwirl,
		cryoSwirl,
		electroCharged,
		shattered,
		overloaded,
		bloom,
		burgeon,
		hyperbloom,
		lunarCharged,
	};

	enum class NodeReaction : uint8_t {
		none,

		forwardVape,
		forwardMelt,
		reverseVape,
		reverseMelt,

		aggravate,
		spread,
	};

	const inline std::vector<Reaction> reactions{
		Reaction::vape,
		Reaction::melt,
		Reaction::aggravate,
		Reaction::spread,
		Reaction::burning,
		Reaction::superconduct,
		Reaction::swirl,
		Reaction::electroCharged,
		Reaction::shattered,
		Reaction::overloaded,
		Reaction::bloom,
		Reaction::burgeon,
		Reaction::hyperbloom,
	};

	const inline std::vector<TransformativeReaction> transformativeReactions{
		TransformativeReaction::burning,
		TransformativeReaction::superconduct,
		TransformativeReaction::pyroSwirl,
		TransformativeReaction::hydroSwirl,
		TransformativeReaction::electroSwirl,
		TransformativeReaction::cryoSwirl,
		TransformativeReaction::electroCharged,
		TransformativeReaction::shattered,
		TransformativeReaction::overloaded,
		TransformativeReaction::bloom,
		TransformativeReaction::burgeon,
		TransformativeReaction::hyperbloom,

		TransformativeReaction::lunarCharged,
	};

	const inline std::vector<NodeReaction> nodeReactions{
		NodeReaction::none,
		NodeReaction::forwardVape,
		NodeReaction::forwardMelt,
		NodeReaction::reverseVape,
		NodeReaction::reverseMelt,
		NodeReaction::aggravate,
		NodeReaction::spread,
	};
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::Reaction &reaction) {
		switch (reaction) {
			case Misc::Reaction::vape:
				return "Vaporize";
			case Misc::Reaction::melt:
				return "Melt";
			case Misc::Reaction::aggravate:
				return "Aggravate";
			case Misc::Reaction::spread:
				return "Spread";
			case Misc::Reaction::burning:
				return "Burning";
			case Misc::Reaction::superconduct:
				return "Superconduct";
			case Misc::Reaction::swirl:
				return "Swirl";
			case Misc::Reaction::electroCharged:
				return "Electro-Charged";
			case Misc::Reaction::shattered:
				return "Shattered";
			case Misc::Reaction::overloaded:
				return "Overloaded";
			case Misc::Reaction::bloom:
				return "Bloom";
			case Misc::Reaction::burgeon:
				return "Burgeon";
			case Misc::Reaction::hyperbloom:
				return "Hyperbloom";
			case Misc::Reaction::lunarCharged:
				return "Lunar-Charged";
		}
		std::unreachable();
	}

	template<>
	constexpr std::string Stringify<>(const Misc::TransformativeReaction &reaction) {
		switch (reaction) {
			case Misc::TransformativeReaction::burning:
				return "Burning";
			case Misc::TransformativeReaction::superconduct:
				return "Superconduct";
			case Misc::TransformativeReaction::pyroSwirl:
				return "Pyro Swirl";
			case Misc::TransformativeReaction::hydroSwirl:
				return "Hydro Swirl";
			case Misc::TransformativeReaction::electroSwirl:
				return "Electro Swirl";
			case Misc::TransformativeReaction::cryoSwirl:
				return "Cryo Swirl";
			case Misc::TransformativeReaction::electroCharged:
				return "Electro-Charged";
			case Misc::TransformativeReaction::shattered:
				return "Shattered";
			case Misc::TransformativeReaction::overloaded:
				return "Overloaded";
			case Misc::TransformativeReaction::bloom:
				return "Bloom";
			case Misc::TransformativeReaction::burgeon:
				return "Burgeon";
			case Misc::TransformativeReaction::hyperbloom:
				return "Hyperbloom";
			case Misc::TransformativeReaction::lunarCharged:
				return "Lunar-Charged";
		}
		std::unreachable();
	}
}// namespace Utils