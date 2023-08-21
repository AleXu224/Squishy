#pragma once

#include "element.hpp"
#include "stat.hpp"
#include "talent.hpp"
#include <array>
#include <functional>
#include <utility>
#include "stdexcept"

namespace Squishy {
	struct StatSheet {

		// Stat structure:
		// Value - The accumulated values known at initialization time
		//         Could be from weapons, talents which contain a constant value
		//         (talent levels are known at creation time), constellations, etc.
		//         Could also be functions that depend on other stats but are known at
		//         initialization time
		// Modifiers - The functions that modify the value at runtime
		//             These could be from talents (which depend on other stats),
		//             constellations, etc. These will only be touched when the
		//             StatSheet is created
		struct Stat {
			uint8_t modifierCount{0};
			uint8_t totalModifierCount{0};
			uint8_t artifactModifierCount{0};
			float value{0};
			std::array<std::function<float(const StatSheet &)>, 8> modifiers{};
			std::array<std::function<float(const StatSheet &)>, 8> totalModifiers{};
			float artifactValue{0};
			std::array<std::function<float(const StatSheet &)>, 4> artifactModifiers{};

#ifndef NDEBUG
			mutable bool inside = false;
#endif

			[[nodiscard]] inline float get(const StatSheet &statSheet) const {
#ifndef NDEBUG
				if (inside) {
					throw std::runtime_error("Circular dependency detected");
				}
				inside = true;
#endif
				float result = value + artifactValue;
				for (uint8_t i = 0; i < modifierCount; ++i) {
					result += modifiers[i](statSheet);
				}
				for (uint8_t i = 0; i < totalModifierCount; ++i) {
					result += totalModifiers[i](statSheet);
				}
#ifndef NDEBUG
				inside = false;
#endif
				return result;
			}

			[[nodiscard]] inline float getTotal(const StatSheet &statSheet) const {
				// Way faster to repeat the code than to call get()
				float result = value + artifactValue;
				// constexpr auto noop = [](const StatSheet &) { return 0; };
				for (uint8_t i = 0; i < modifierCount; ++i) {
					result += modifiers[i](statSheet);
				}
				for (uint8_t i = 0; i < totalModifierCount; ++i) {
					result += totalModifiers[i](statSheet);
				}
				for (uint8_t i = 0; i < artifactModifierCount; ++i) {
					result += artifactModifiers[i](statSheet);
				}

				return result;
			}

			inline void operator+=(const float &modifier) {
				value += modifier;
			}

			inline void operator+=(const std::function<float(const StatSheet &)> &&modifier) {
				modifiers[modifierCount++] = modifier;
			}

			inline void addTotal(const std::function<float(const StatSheet &)> &&modifier) {
				totalModifiers[totalModifierCount++] = modifier;
			}

			inline void clear() {
				modifierCount = 0;
				totalModifierCount = 0;
				value = 0;
			}

			inline void clearArtifacts() {
				artifactModifierCount = 0;
				artifactValue = 0;
			}
		};

		struct Base {
			float HP{0};
			float ATK{0};
			float DEF{0};
		};
		Base base;

		CharacterTalents talents;
		uint8_t level{1};
		uint8_t ascension{0};
		uint8_t constellation{0};
		uint8_t weaponLevel{1};
		uint8_t weaponAscension{0};
		uint8_t weaponRefinement{1};
		Element element{Element::Anemo};

		struct Stats {
			Stat HP{
				.modifierCount = 1,
				.modifiers = {
					[](const StatSheet &stats) -> float {
						return stats.base.HP * (1.f + stats.stats.HPPercent.getTotal(stats));
					},
				},
			};
			Stat HPPercent;

			Stat ATK{
				.modifierCount = 1,
				.modifiers = {
					[](const StatSheet &stats) -> float {
						return stats.base.ATK * (1.f + stats.stats.ATKPercent.getTotal(stats));
					},
				},
			};
			Stat ATKPercent;

			Stat DEF{
				.modifierCount = 1,
				.modifiers = {
					[](const StatSheet &stats) -> float {
						return stats.base.DEF * (1.f + stats.stats.DEFPercent.getTotal(stats));
					},
				},
			};
			Stat DEFPercent;

			Stat EM;
			Stat ER;
			Stat HB;
			Stat IncomingHB;

			struct SkillStats {
				Stat DMG;
				Stat AdditiveDMG;
				Stat CritRate;
				Stat CritDMG;
			};
			SkillStats Anemo;
			SkillStats Cryo;
			SkillStats Dendro;
			SkillStats Electro;
			SkillStats Geo;
			SkillStats Hydro;
			SkillStats Pyro;
			SkillStats Physical;
			SkillStats All;

			SkillStats Normal;
			SkillStats Charged;
			SkillStats Plunge;
			SkillStats Skill;
			SkillStats Burst;

			inline void clear() {
				// The first three stats need to include the base value
				// Clearing then readding the function to calculate would be pretty slow
				// Instead the first modifier is guaranteed to be the needed function
				// so setting the modifier count to 1 will be a lot faster
				HP.clear();
				HP.modifierCount = 1;
				HPPercent.clear();

				ATK.clear();
				ATK.modifierCount = 1;
				ATKPercent.clear();

				DEF.clear();
				DEF.modifierCount = 1;
				DEFPercent.clear();

				EM.clear();
				ER.clear();
				HB.clear();
				IncomingHB.clear();

				constexpr auto clearSkillStats = [](SkillStats &skillStats) {
					skillStats.DMG.clear();
					skillStats.AdditiveDMG.clear();
					skillStats.CritRate.clear();
					skillStats.CritDMG.clear();
				};
				clearSkillStats(Anemo);
				clearSkillStats(Cryo);
				clearSkillStats(Dendro);
				clearSkillStats(Electro);
				clearSkillStats(Geo);
				clearSkillStats(Hydro);
				clearSkillStats(Pyro);
				clearSkillStats(Physical);
				clearSkillStats(All);

				clearSkillStats(Normal);
				clearSkillStats(Charged);
				clearSkillStats(Plunge);
				clearSkillStats(Skill);
				clearSkillStats(Burst);
			}

			inline void clearArtifacts() {
				HP.clearArtifacts();
				HPPercent.clearArtifacts();

				ATK.clearArtifacts();
				ATKPercent.clearArtifacts();

				DEF.clearArtifacts();
				DEFPercent.clearArtifacts();

				EM.clearArtifacts();
				ER.clearArtifacts();
				HB.clearArtifacts();
				IncomingHB.clearArtifacts();

				constexpr auto clearSkillStats = [](SkillStats &skillStats) {
					skillStats.DMG.clearArtifacts();
					skillStats.AdditiveDMG.clearArtifacts();
					skillStats.CritRate.clearArtifacts();
					skillStats.CritDMG.clearArtifacts();
				};
				clearSkillStats(Anemo);
				clearSkillStats(Cryo);
				clearSkillStats(Dendro);
				clearSkillStats(Electro);
				clearSkillStats(Geo);
				clearSkillStats(Hydro);
				clearSkillStats(Pyro);
				clearSkillStats(Physical);
				clearSkillStats(All);

				clearSkillStats(Normal);
				clearSkillStats(Charged);
				clearSkillStats(Plunge);
				clearSkillStats(Skill);
				clearSkillStats(Burst);
			}

			[[nodiscard]] inline Stat &getStat(ArtifactStat stat) {
				switch (stat) {
					case ArtifactStat::HP:
						return HP;
					case ArtifactStat::HPPercent:
						return HPPercent;
					case ArtifactStat::ATK:
						return ATK;
					case ArtifactStat::ATKPercent:
						return ATKPercent;
					case ArtifactStat::DEF:
						return DEF;
					case ArtifactStat::DEFPercent:
						return DEFPercent;
					case ArtifactStat::EM:
						return EM;
					case ArtifactStat::ER:
						return ER;
					case ArtifactStat::HB:
						return HB;
					case ArtifactStat::CritRate:
						return All.CritRate;
					case ArtifactStat::CritDMG:
						return All.CritDMG;
					case ArtifactStat::AnemoDMG:
						return Anemo.DMG;
					case ArtifactStat::CryoDMG:
						return Cryo.DMG;
					case ArtifactStat::DendroDMG:
						return Dendro.DMG;
					case ArtifactStat::ElectroDMG:
						return Electro.DMG;
					case ArtifactStat::GeoDMG:
						return Geo.DMG;
					case ArtifactStat::HydroDMG:
						return Hydro.DMG;
					case ArtifactStat::PyroDMG:
						return Pyro.DMG;
					case ArtifactStat::PhysicalDMG:
						return Physical.DMG;
					default:
						std::unreachable();
				}
			}

			[[nodiscard]] inline SkillStats fromElement(Element element) const {
				switch (element) {
					case Element::Anemo:
						return Anemo;
					case Element::Cryo:
						return Cryo;
					case Element::Dendro:
						return Dendro;
					case Element::Electro:
						return Electro;
					case Element::Geo:
						return Geo;
					case Element::Hydro:
						return Hydro;
					case Element::Pyro:
						return Pyro;
					default:
						std::unreachable();
				}
			}
		} stats;
	};
	using StatModifier = std::function<float(const StatSheet &)>;
}// namespace Squishy