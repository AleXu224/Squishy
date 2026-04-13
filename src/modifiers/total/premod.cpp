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
	struct PreModFrm : Formula::FormulaBase<float> {
		Formula::FloatNode characterKitStat;
		Formula::FloatNode characterInstanceStat;
		Formula::FloatNode weaponPassiveStat;
		Formula::FloatNode weaponInstanceStat;
		Formula::FloatNode artifactSetStat;
		Formula::FloatNode artifactSubStats;
		Formula::FloatNode teamPreStat;
		Formula::FloatNode activePreStat;
		Formula::FloatNode teamResonances;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = characterKitStat
					 + characterInstanceStat
					 + weaponPassiveStat
					 + weaponInstanceStat
					 + artifactSetStat
					 + artifactSubStats
					 + teamPreStat
					 + activePreStat
					 + teamResonances;
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (characterKitStat
					+ characterInstanceStat
					+ weaponPassiveStat
					+ weaponInstanceStat
					+ artifactSetStat
					+ artifactSubStats
					+ teamPreStat
					+ activePreStat
					+ teamResonances)
				.print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return characterKitStat.eval(context)
				 + characterInstanceStat.eval(context)
				 + weaponPassiveStat.eval(context)
				 + weaponInstanceStat.eval(context)
				 + artifactSetStat.eval(context)
				 + artifactSubStats.eval(context)
				 + teamPreStat.eval(context)
				 + activePreStat.eval(context)
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
			Team::activePreMods(),
			Team::resonances()
		);
		return ret;
	}
}// namespace Modifiers