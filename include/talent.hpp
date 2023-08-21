#pragma once
#include "cstdint"

namespace Squishy {
    struct CharacterTalents {
        uint8_t normal{0};
        uint8_t skill{0};
        uint8_t burst{0};
    };
    
    enum class Talent {
        Normal,
        Charged,
        Plunge,
        Skill,
        Burst,
        Passive1,
        Passive2,
        Passive3,
        Constellation1,
        Constellation2,
        Constellation3,
        Constellation4,
        Constellation5,
        Constellation6,
    };

    enum class LevelableTalent {
        Normal,
        Skill,
        Burst,
    };
}