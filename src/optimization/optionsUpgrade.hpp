#pragma once

#include "combo/source.hpp"
#include "optional"
#include <cstdint>
#include <map>


namespace Optimization {
	struct OptionsUpgrade {
		std::map<Stat, bool> sandsMainStats{};
		std::map<Stat, bool> gobletMainStats{};
		std::map<Stat, bool> circletMainStats{};
		std::optional<Combo::Source::Types> nodeSource;
		bool enableLevelUp = true;
		bool enableReroll = true;
		bool enableDefinition = true;
		uint8_t rerollGuaranteedRolls = 2;
		std::map<Stat, bool> guaranteedSubStats{};
	};
}// namespace Optimization