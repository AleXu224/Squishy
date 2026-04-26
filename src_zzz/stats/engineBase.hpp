#pragma once

#include "misc/curves.hpp"
#include "misc/specialty.hpp"
#include "stats/stat.hpp"


namespace Stats {
	struct EngineBase {
		const Misc::Specialty specialty;
		const uint8_t rarity;
		const float baseAtk;
		StatValue subStat;

		[[nodiscard]] float getAtkAt(unsigned short level, unsigned short modification) const {
			return baseAtk * (1.f + Curves::engineAtk.at(level) + Curves::engineLevelConstant * modification);
		}

		[[nodiscard]] float getSubstatAt(unsigned short level, unsigned short modification) const {
			return subStat.value * (1.f + 0.3f * modification);
		}
	};
}// namespace Stats
