#pragma once

#include "IAgent.hpp"
#include "IDisc.hpp"
#include "IEngine.hpp"
#include "cstdint"
#include "string"
#include "vector"


namespace Serialization::Zod {
	struct IZOD {
		std::string format = "ZOD";
		uint8_t version = 1;
		std::string source = "Squishy";
		std::optional<std::vector<IAgent>> characters = std::nullopt;
		std::optional<std::vector<IDisc>> discs = std::nullopt;
		std::optional<std::vector<IEngine>> wengines = std::nullopt;
	};
}// namespace Serialization::Zod