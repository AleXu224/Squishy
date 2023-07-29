#pragma once

namespace Squishy {
    constexpr float operator""_p (long double percent) {
        return static_cast<float>(percent) / 100.0f;
    }

    enum class ArtifactStat {
        HP,
        HPPercent,

        ATK,
        ATKPercent,

        DEF,
        DEFPercent,

        EM,
        ER,
        HB,

        CritRate,
        CritDMG,

        AnemoDMG,
        CryoDMG,
        DendroDMG,
        ElectroDMG,
        GeoDMG,
        HydroDMG,
        PyroDMG,
        PhysicalDMG,
    };

    enum class AbilityScalingStat {
        HP,
        ATK,
        DEF,
        EM,
    };
}