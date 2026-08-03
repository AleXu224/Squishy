#pragma once

#include "combo/source.hpp"
#include "optional"
#include <map>


namespace Optimization {
	struct OptionsUpgrade {
		std::map<Stat, bool> sandsMainStats{};
		std::map<Stat, bool> gobletMainStats{};
		std::map<Stat, bool> circletMainStats{};
		std::optional<Combo::Source::Types> nodeSource;
	};
}// namespace Optimization