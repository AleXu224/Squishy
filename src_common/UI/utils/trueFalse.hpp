#pragma once

#include "array"
#include "ranges"

namespace Utils {
	const inline std::array<bool, 2> _trueFalse{false, true};
	const inline auto trueFalse = std::views::join(std::views::repeat(_trueFalse));
}// namespace Utils