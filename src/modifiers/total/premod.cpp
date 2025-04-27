#include "premod.hpp"
#include "formula/operators.hpp"
#include "modifiers/artifact/set.hpp"
#include "modifiers/artifact/subStats.hpp"
#include "modifiers/character/instance.hpp"
#include "modifiers/character/kit.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/team/resonance.hpp"
#include "modifiers/team/team.hpp"
#include "modifiers/weapon/instance.hpp"
#include "modifiers/weapon/passive.hpp"


namespace Modifiers {
	using namespace Formula::Operators;
	struct PreModFrm {
		const Formula::FloatNode &characterKitStat;
		const Formula::FloatNode &characterInstanceStat;
		const Formula::FloatNode &weaponPassiveStat;
		const Formula::FloatNode &weaponInstanceStat;
		const Formula::FloatNode &artifactSetStat;
		const Formula::FloatNode &artifactSubStats;
		const Formula::FloatNode &teamPreStat;
		const Formula::FloatNode &teamResonances;
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

	const Stats::Sheet<Formula::FloatNode> &preMods() {
		static auto ret = statFactory<Formula::FloatNode, PreModFrm>(
			Character::Kit::preMods(),
			Character::instanceStats(),
			Weapon::Passive::preMods(),
			Weapon::instanceStats(),
			Artifact::Set::preMods(),
			Artifact::subStats(),
			Team::preMods(),
			Team::resonances()
		);
		return ret;
	}
}// namespace Modifiers