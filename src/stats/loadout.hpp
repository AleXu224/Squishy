#pragma once

#include "stats/character.hpp"
#include "stats/weapon.hpp"
#include "stats/artifact.hpp"

namespace Stats {
    struct Loadout{
        Stats::Character character;
        Stats::Weapon &weapon;
        Stats::Artifact artifact;
    };
}