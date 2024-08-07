#pragma once

#include "stats/character.hpp"
#include "stats/weapon.hpp"
#include "stats/artifact.hpp"

namespace Stats {
    struct Sheet{
        Stats::Character character;
        Stats::Weapon &weapon;
        Stats::Artifact artifact;
    };
}