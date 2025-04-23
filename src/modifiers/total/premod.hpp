#pragma once

#include "formula/operators.hpp"// IWYU pragma: keep
#include "modifiers/artifact/set.hpp"
#include "modifiers/artifact/subStats.hpp"
#include "modifiers/character/instance.hpp"
#include "modifiers/character/kit.hpp"
#include "modifiers/team/resonance.hpp"
#include "modifiers/team/team.hpp"
#include "modifiers/weapon/instance.hpp"
#include "modifiers/weapon/passive.hpp"


namespace Modifiers {
	using namespace Formula::Operators;
	template<auto characterKitStat, auto characterInstanceStat, auto weaponPassiveStat, auto weaponInstanceStat, auto artifactSetStat, auto artifactSubStats, auto teamPreStat, auto teamResonances>
	struct PreModFrm {
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return (characterKitStat + characterInstanceStat + weaponPassiveStat + weaponInstanceStat + artifactSetStat + artifactSubStats + teamPreStat + teamResonances).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (characterKitStat + characterInstanceStat + weaponPassiveStat + weaponInstanceStat + artifactSetStat + artifactSubStats + teamPreStat).print(context, prevStep);
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return characterKitStat.eval(context)
				 + characterInstanceStat.eval(context)
				 + weaponPassiveStat.eval(context)
				 + weaponInstanceStat.eval(context)
				 + artifactSetStat.eval(context)
				 + artifactSubStats.eval(context)
				 + teamPreStat.eval(context)
				 + teamResonances.eval(context);
		}
	};

	static constexpr StatFactory<PreModFrm, Character::Kit::preMods, Character::instanceStats, Weapon::Passive::preMods, Weapon::instanceStats, Artifact::Set::preMods, Artifact::subStats, Team::preMods, Team::resonances> preMods;
}// namespace Modifiers