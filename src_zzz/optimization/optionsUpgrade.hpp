#pragma once

#include "combo/source.hpp"
#include "optional"
#include <map>

namespace Optimization {
	struct OptionsUpgrade {
		std::map<Stat, bool> partition4MainStats{};
		std::map<Stat, bool> partition5MainStats{};
		std::map<Stat, bool> partition6MainStats{};
		std::optional<Combo::Source::Types> nodeSource;
		bool enableLevelUp = true;
		bool enableDefinition = true;
		std::map<Stat, bool> guaranteedSubStats{};
	};
}// namespace Optimization
