#pragma once

#include "engineSheet.hpp"


namespace Engine {
	struct Data;
}

namespace Stats {
	struct Engine {
		const ::Engine::Data *data;
		Stats::EngineSheet sheet;

		Engine(const ::Engine::Data &data);
	};
}// namespace Stats