#pragma once

#include "array"
#include "cstdint"
#include "utility"
#include "utils/stringify.hpp"


namespace Misc {
	enum class DamageType : uint8_t {
		sheer,
	};

	static constexpr auto damageTypes = std::array<Misc::DamageType, 1>{
		DamageType::sheer,
	};
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::DamageType &damageType) {
		switch (damageType) {
			case Misc::DamageType::sheer:
				return "Sheer";
		}

		std::unreachable();
	}
}// namespace Utils