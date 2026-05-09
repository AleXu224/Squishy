#include "sets.hpp"

#include "disc/sets/FangedMetal.hpp"
#include "disc/sets/FreedomBlues.hpp"
#include "disc/sets/PhaethonsMelody.hpp"
#include "disc/sets/WoodpeckerElectro.hpp"


void Disc::initDiscs() {
	sets = {
		{Sets::fangedMetal.key, Sets::fangedMetal},
		{Sets::freedomBlues.key, Sets::freedomBlues},
		{Sets::phaethonsMelody.key, Sets::phaethonsMelody},
		{Sets::woodpeckerElectro.key, Sets::woodpeckerElectro},
	};
}
