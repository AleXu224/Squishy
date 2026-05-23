#include "sets.hpp"

#include "disc/sets/AstralVoice.hpp"
#include "disc/sets/BranchBladeSong.hpp"
#include "disc/sets/FangedMetal.hpp"
#include "disc/sets/FreedomBlues.hpp"
#include "disc/sets/MoonlightLullaby.hpp"
#include "disc/sets/PhaethonsMelody.hpp"
#include "disc/sets/PolarMetal.hpp"
#include "disc/sets/PufferElectro.hpp"
#include "disc/sets/WoodpeckerElectro.hpp"


void Disc::initDiscs() {
	sets = {
		{Sets::astralVoice.key, Sets::astralVoice},
		{Sets::branchBladeSong.key, Sets::branchBladeSong},
		{Sets::fangedMetal.key, Sets::fangedMetal},
		{Sets::freedomBlues.key, Sets::freedomBlues},
		{Sets::moonlightLullaby.key, Sets::moonlightLullaby},
		{Sets::phaethonsMelody.key, Sets::phaethonsMelody},
		{Sets::polarMetal.key, Sets::polarMetal},
		{Sets::pufferElectro.key, Sets::pufferElectro},
		{Sets::woodpeckerElectro.key, Sets::woodpeckerElectro},
	};
}
