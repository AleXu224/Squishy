#include "engines.hpp"

#include "engine/engines/PracticedPerfection.hpp"

void Engine::initEngines() {
	list = {
		{Engine::Datas::practicedPerfection.key, Engine::Datas::practicedPerfection},
	};
}
