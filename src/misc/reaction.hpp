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

	static inline std::vector<Reaction>
		reactions{
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

	static inline std::vector<NodeReaction> nodeReactions{
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
		}
		std::unreachable();
	}
}// namespace Utils