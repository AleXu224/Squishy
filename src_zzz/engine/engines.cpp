#include "engines.hpp"

#include "engine/engines/ChiefSidekick.hpp"
#include "engine/engines/FusionCompiler.hpp"
#include "engine/engines/HailstormShrine.hpp"
#include "engine/engines/HellfireGears.hpp"
#include "engine/engines/JoyauDore.hpp"
#include "engine/engines/KaboomTheCannon.hpp"
#include "engine/engines/KnightsExtolment.hpp"
#include "engine/engines/Metanukimorphosis.hpp"
#include "engine/engines/NeonFantasies.hpp"
#include "engine/engines/PracticedPerfection.hpp"
#include "engine/engines/QingmingBirdcage.hpp"
#include "engine/engines/TheBrimstone.hpp"
#include "engine/engines/Thoughtbop.hpp"
#include "engine/engines/WeepingGemini.hpp"


void Engine::initEngines() {
	list = {
		{Engine::Datas::chiefSidekick.key, Engine::Datas::chiefSidekick},
		{Engine::Datas::fusionCompiler.key, Engine::Datas::fusionCompiler},
		{Engine::Datas::hailstormShrine.key, Engine::Datas::hailstormShrine},
		{Engine::Datas::hellfireGears.key, Engine::Datas::hellfireGears},
		{Engine::Datas::joyauDore.key, Engine::Datas::joyauDore},
		{Engine::Datas::kaboomTheCannon.key, Engine::Datas::kaboomTheCannon},
		{Engine::Datas::knightsExtolment.key, Engine::Datas::knightsExtolment},
		{Engine::Datas::metanukimorphosis.key, Engine::Datas::metanukimorphosis},
		{Engine::Datas::neonFantasies.key, Engine::Datas::neonFantasies},
		{Engine::Datas::practicedPerfection.key, Engine::Datas::practicedPerfection},
		{Engine::Datas::qingmingBirdcage.key, Engine::Datas::qingmingBirdcage},
		{Engine::Datas::theBrimstone.key, Engine::Datas::theBrimstone},
		{Engine::Datas::thoughtbop.key, Engine::Datas::thoughtbop},
		{Engine::Datas::weepingGemini.key, Engine::Datas::weepingGemini},
	};
}
