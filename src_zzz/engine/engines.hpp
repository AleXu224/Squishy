#pragma once

#include "engine/data.hpp"
#include <unordered_map>

namespace Engine {
	inline std::unordered_map<Engine::DataKey, const Engine::Data &> list;

	extern void initEngines();
}// namespace Engine
