#pragma once

#include <cstdint>
#include <string_view>

namespace Squishy {
	constexpr float operator""_p(long double percent) {
		return static_cast<float>(percent) / 100.0f;
	}

	enum class ArtifactStat : uint8_t {
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

	constexpr std::string_view ArtifactStatToString(const ArtifactStat &stat) {
		switch (stat) {
			case ArtifactStat::HP:
				return "HP";
			case ArtifactStat::HPPercent:
				return "HP%";
			case ArtifactStat::ATK:
				return "ATK";
			case ArtifactStat::ATKPercent:
				return "ATK%";
			case ArtifactStat::DEF:
				return "DEF";
			case ArtifactStat::DEFPercent:
				return "DEF%";
			case ArtifactStat::EM:
				return "Elemental Mastery";
			case ArtifactStat::ER:
				return "Energy Recharge";
			case ArtifactStat::HB:
				return "Healing Bonus";
			case ArtifactStat::CritRate:
				return "Crit Rate";
			case ArtifactStat::CritDMG:
				return "Crit DMG";
			case ArtifactStat::AnemoDMG:
				return "Anemo DMG Bonus";
			case ArtifactStat::CryoDMG:
				return "Cryo DMG Bonus";
			case ArtifactStat::DendroDMG:
				return "Dendro DMG Bonus";
			case ArtifactStat::ElectroDMG:
				return "Electro DMG Bonus";
			case ArtifactStat::GeoDMG:
				return "Geo DMG Bonus";
			case ArtifactStat::HydroDMG:
				return "Hydro DMG Bonus";
			case ArtifactStat::PyroDMG:
				return "Pyro DMG Bonus";
			case ArtifactStat::PhysicalDMG:
				return "Physical DMG Bonus";
			default:
				return "Unknown";
		}
	}

    constexpr bool ArtifactStatIsPercentage(const ArtifactStat &stat) {
        switch (stat) {
            case ArtifactStat::HPPercent:
            case ArtifactStat::ATKPercent:
            case ArtifactStat::DEFPercent:
            case ArtifactStat::ER:
            case ArtifactStat::HB:
            case ArtifactStat::CritRate:
            case ArtifactStat::CritDMG:
            case ArtifactStat::AnemoDMG:
            case ArtifactStat::CryoDMG:
            case ArtifactStat::DendroDMG:
            case ArtifactStat::ElectroDMG:
            case ArtifactStat::GeoDMG:
            case ArtifactStat::HydroDMG:
            case ArtifactStat::PyroDMG:
            case ArtifactStat::PhysicalDMG:
                return true;
            default:
                return false;
        }
    }

	enum class AbilityScalingStat {
		HP,
		ATK,
		DEF,
		EM,
	};
}// namespace Squishy