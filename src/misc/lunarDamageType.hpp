#pragma once

#include "cstdint"
#include "misc/element.hpp"
#include "string"
#include "utils/stringify.hpp"
#include <vector>

namespace Misc {
	enum class LunarDamageType : uint8_t {
		lunarCharged,
		lunarBloom,
		lunarCrystalize,
	};

	static inline std::vector<LunarDamageType> lunarDamageTypes{
		LunarDamageType::lunarCharged,
		LunarDamageType::lunarBloom,
		LunarDamageType::lunarCrystalize,
	};

	static inline Misc::Element lunarDamageTypeToElement(LunarDamageType type) {
		switch (type) {
			case LunarDamageType::lunarCharged:
				return Misc::Element::electro;
			case LunarDamageType::lunarBloom:
				return Misc::Element::dendro;
			case LunarDamageType::lunarCrystalize:
				return Misc::Element::geo;
		}
		std::unreachable();
	}
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::LunarDamageType &element) {
		switch (element) {
			case Misc::LunarDamageType::lunarCharged:
				return "Lunar-Charged";
			case Misc::LunarDamageType::lunarBloom:
				return "Lunar-Bloom";
			case Misc::LunarDamageType::lunarCrystalize:
				return "Lunar-Crystalize";
		}
		std::unreachable();
	}
}// namespace Utils