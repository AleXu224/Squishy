#pragma once

#include "cstdint"

namespace Misc {
	enum class AttackSource : uint8_t {
		normal,
		charged,
		plunge,
		skill,
		burst
	};
}