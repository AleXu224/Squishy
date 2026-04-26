#pragma once

#include "stats/sheet.hpp"
#include "engineBase.hpp"
#include "value.hpp"


namespace Stats {
	struct Loadout;
	struct Engine;
	struct EngineSheet {
		uint8_t level{1};
		uint8_t modification{0};
		uint8_t overclock{1};

		[[nodiscard]] float getBaseAtk();
		[[nodiscard]] float getSubstat();

		Stats::Sheet<Stats::Value<float, 1>> base{};
		Stats::Sheet<Stats::Value<float, 1>> initial{};

		explicit EngineSheet(const EngineBase &base);
	};
}// namespace Stats
