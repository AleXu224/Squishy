#pragma once

#include "string"
#include <optional>

namespace Serialization::Good {
	struct ISubstat {
		std::string key;
		float value;
		std::optional<float> initialValue = std::nullopt;
	};
}// namespace Serialization::Good
