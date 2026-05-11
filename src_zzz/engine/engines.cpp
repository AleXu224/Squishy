#include "engines.hpp"

#include "engine/engines/KaboomTheCannon.hpp"
#include "engine/engines/PracticedPerfection.hpp"
#include "engine/engines/WeepingGemini.hpp"


void Engine::initEngines() {
	list = {
		{Engine::Datas::kaboomTheCannon.key, Engine::Datas::kaboomTheCannon},
		{Engine::Datas::practicedPerfection.key, Engine::Datas::practicedPerfection},
		{Engine::Datas::weepingGemini.key, Engine::Datas::weepingGemini},
	};
}
