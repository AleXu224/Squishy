#pragma once

#include "cstdint"
#include "misc/element.hpp"
#include "string"
#include "utils/stringify.hpp"
#include <vector>

namespace Misc {
	enum class StellarDamageType : uint8_t {
		electroStellarConduct,
		cryoStellarConduct,
		stellarSwirl,
	};

	static inline std::vector<StellarDamageType> stellarDamageTypes{
		StellarDamageType::electroStellarConduct,
		StellarDamageType::cryoStellarConduct,
		StellarDamageType::stellarSwirl,
	};

	static inline Misc::Element stellarDamageTypeToElement(StellarDamageType type) {
		switch (type) {
			case StellarDamageType::electroStellarConduct:
				return Misc::Element::electro;
			case StellarDamageType::cryoStellarConduct:
				return Misc::Element::cryo;
			case StellarDamageType::stellarSwirl:
				return Misc::Element::anemo;
		}
		std::unreachable();
	}
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::StellarDamageType &element) {
		switch (element) {
			case Misc::StellarDamageType::electroStellarConduct:
			case Misc::StellarDamageType::cryoStellarConduct:
				return "Stellar-Conduct";
			case Misc::StellarDamageType::stellarSwirl:
				return "Stellar-Swirl";
		}
		std::unreachable();
	}
}// namespace Utils