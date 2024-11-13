#pragma once

#include "array"
#include "cstdint"
#include "utility"
#include "utils/stringify.hpp"


namespace Misc {
	enum class WeaponType : uint8_t {
		sword,
		claymore,
		polearm,
		catalyst,
		bow,
	};

	static constexpr auto weaponTypes = std::array<Misc::WeaponType, 5>{
		Misc::WeaponType::sword,
		Misc::WeaponType::claymore,
		Misc::WeaponType::polearm,
		Misc::WeaponType::catalyst,
		Misc::WeaponType::bow,
	};
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::WeaponType &weaponType) {
		switch (weaponType) {
			case Misc::WeaponType::sword:
				return "Sword";
			case Misc::WeaponType::claymore:
				return "Claymore";
			case Misc::WeaponType::polearm:
				return "Polearm";
			case Misc::WeaponType::catalyst:
				return "Catalyst";
			case Misc::WeaponType::bow:
				return "Bow";
		}

		std::unreachable();
	}
}// namespace Utils