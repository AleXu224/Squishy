#pragma once

#include "array"
#include "cstdint"
#include "utility"
#include "utils/stringify.hpp"


namespace Misc {
	enum class Specialty : uint8_t {
		attack,
		stun,
		anomaly,
		support,
		defense,
		rupture,
	};

	static constexpr auto specialties = std::array<Misc::Specialty, 6>{
		Specialty::attack,
		Specialty::stun,
		Specialty::anomaly,
		Specialty::support,
		Specialty::defense,
		Specialty::rupture,
	};
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::Specialty &specialty) {
		switch (specialty) {
			case Misc::Specialty::attack:
				return "Attack";
			case Misc::Specialty::stun:
				return "Stun";
			case Misc::Specialty::anomaly:
				return "Anomaly";
			case Misc::Specialty::support:
				return "Support";
			case Misc::Specialty::defense:
				return "Defense";
			case Misc::Specialty::rupture:
				return "Rupture";
		}

		std::unreachable();
	}
}// namespace Utils