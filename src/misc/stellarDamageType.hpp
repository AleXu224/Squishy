#pragma once

#include "cstdint"
#include "string"
#include "utils/stringify.hpp"
#include <vector>

namespace Misc {
	enum class StellarDamageType : uint8_t {
		stellarConduct,
		stellarSwirl,
	};

	static inline std::vector<StellarDamageType> stellarDamageTypes{
		StellarDamageType::stellarConduct,
		StellarDamageType::stellarSwirl,
	};
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::StellarDamageType &element) {
		switch (element) {
			case Misc::StellarDamageType::stellarConduct:
				return "Stellar-Conduct";
			case Misc::StellarDamageType::stellarSwirl:
				return "Stellar-Swirl";
		}
		std::unreachable();
	}
}// namespace Utils