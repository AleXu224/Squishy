#include "sets.hpp"

#include "disc/sets/FangedMetal.hpp"
#include "disc/sets/WoodpeckerElectro.hpp"


void Disc::initDiscs() {
	sets = {
		{Sets::fangedMetal.key, Sets::fangedMetal},
		{Sets::woodpeckerElectro.key, Sets::woodpeckerElectro},
	};
}
