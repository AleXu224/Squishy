#pragma once

#include "formula/percentage.hpp"
#include "modifiers/total/premod.hpp"


namespace Modifiers {
	using namespace Formula::Operators;
	template<auto characterKitStat, auto weaponPassiveStat, auto artifactSetStat, auto teamPostStat, auto preModStat, SheetMemberIdentifier name>
	struct TotalFrm {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(name.getName(), eval(context), name.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return characterKitStat.eval(context)
				 + weaponPassiveStat.eval(context)
				 + artifactSetStat.eval(context)
				 + teamPostStat.eval(context)
				 + preModStat.eval(context);
		}
	};
	template<auto characterKitStat, auto weaponPassiveStat, auto artifactSetStat, auto teamPostStat, auto preModStat, SheetMemberIdentifier name>
	struct DisplayTotalFrm {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (characterKitStat + weaponPassiveStat + artifactSetStat + teamPostStat + preModStat).print(context, prevStep);
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return characterKitStat.eval(context)
				 + weaponPassiveStat.eval(context)
				 + artifactSetStat.eval(context)
				 + teamPostStat.eval(context)
				 + preModStat.eval(context);
		}
	};
	template<auto characterKitTalent, auto characterInstanceTalent, auto weaponPassiveTalent, auto artifactSetTalent, auto teamTalent>
	struct TotalTalentsFrm {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (characterKitTalent + characterInstanceTalent + weaponPassiveTalent + artifactSetTalent + teamTalent).print(context, prevStep);
		}

		[[nodiscard]] constexpr auto eval(const Formula::Context &context) const {
			return characterKitTalent.eval(context)
				 + characterInstanceTalent.eval(context)
				 + weaponPassiveTalent.eval(context)
				 + artifactSetTalent.eval(context)
				 + teamTalent.eval(context);
		}
	};

	static constexpr StatFactory<TotalFrm, Character::Kit::postMods, Weapon::Passive::postMods, Artifact::Set::postMods, Team::postMods, preMods, StatNameFactory{}> total;
	static constexpr StatFactory<DisplayTotalFrm, Character::Kit::postMods, Weapon::Passive::postMods, Artifact::Set::postMods, Team::postMods, preMods, StatNameFactory{}> displayTotal;
	static constexpr TalentFactory<TotalTalentsFrm, Character::Kit::talents, Character::instanceTalents, Weapon::Passive::talents, Artifact::Set::talents, Team::talents> totalTalents;
	static constexpr auto totalEnemy = Team::enemy;
}// namespace Modifiers