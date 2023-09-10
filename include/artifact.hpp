#pragma once

#include "stat.hpp"
#include <array>
#include <cstdint>

namespace Squishy {
    enum class ArtifactSlot : uint8_t {
        Flower,
        Plume,
        Sands,
        Goblet,
        Circlet,
    };
    
    struct Artifact {
        uint8_t level = 1;
        uint8_t rarity = 1;
        ArtifactSlot slot = ArtifactSlot::Flower;
        struct SubStat {
            ArtifactStat stat = ArtifactStat::HP;
            float value = 0.f;
        };
        SubStat mainStat {
            .stat = ArtifactStat::HP,
            .value = 0.f,
        };
        std::array<SubStat, 4> subStats{};
    };
}