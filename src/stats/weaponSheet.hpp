#pragma once

#include "value.hpp"
#include "weaponBase.hpp"

namespace Stats {
	struct WeaponSheet {
		unsigned short level{1};
		uint8_t ascension{0};
		unsigned short refinement{1};

		Value<WeaponSheet> atk{0};
		Value<WeaponSheet> subStat{0};

		explicit WeaponSheet(const WeaponBase &base) {
			atk.modifiers.emplace_back([&base](const WeaponSheet &sheet) {
				return base.getAtkAt(sheet.level, sheet.ascension);
			});
			subStat.modifiers.emplace_back([&base](const WeaponSheet &sheet) {
				return base.getSubstatAt(sheet.level);
			});
		}
	};
}// namespace Stats
