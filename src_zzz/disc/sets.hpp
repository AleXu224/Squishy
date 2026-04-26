#pragma once

#include "set.hpp"

#include "unordered_map"

namespace Disc {
	inline std::unordered_map<Disc::SetKey, const Disc::Set &> sets{};

	extern void initDiscs();
}// namespace Disc
