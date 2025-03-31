#pragma once

#include "formula/percentage.hpp"
#include "modifiers/enemy/instance.hpp"
#include "modifiers/team/resonance.hpp"
#include "modifiers/total/premod.hpp"


namespace Modifiers {
	using namespace Formula::Operators;
	template<auto characterKitStat, auto weaponPassiveStat, auto artifactSetStat, auto teamPostStat, auto preModStat, SheetMemberIdentifier name>
	struct TotalFrm {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(name.getName(), eval(context), name.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			auto characterKit = characterKitStat.eval(context);
			auto weaponPassive = weaponPassiveStat.eval(context);
			auto artifactSet = artifactSetStat.eval(context);
			auto teamPost = teamPostStat.eval(context);
			auto preMod = preModStat.eval(context);
			return characterKit
				 + weaponPassive
				 + artifactSet
				 + teamPost
				 + preMod;
		}
	};
	template<auto characterKitStat, auto weaponPassiveStat, auto artifactSetStat, auto teamPostStat, auto preModStat, SheetMemberIdentifier name>
	struct TotalActiveFrm {
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
			auto newContext = context.withSource(context.active);
			return characterKitStat.eval(newContext)
				 + weaponPassiveStat.eval(newContext)
				 + artifactSetStat.eval(newContext)
				 + teamPostStat.eval(newContext)
				 + preModStat.eval(newContext);
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
	template<auto teamStat, auto teamResonanceStat, auto instanceStat, SheetMemberIdentifier name>
	struct TotalEnemyFrm {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(name.getName(), eval(context), name.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return teamStat.eval(context)
				 + teamResonanceStat.eval(context)
				 + instanceStat.eval(context);
		}
	};

	static constexpr StatFactory<TotalFrm, Character::Kit::postMods, Weapon::Passive::postMods, Artifact::Set::postMods, Team::postMods, preMods, StatNameFactory{}> total;
	static constexpr StatFactory<TotalActiveFrm, Character::Kit::postMods, Weapon::Passive::postMods, Artifact::Set::postMods, Team::postMods, preMods, StatNameFactory{}> totalActive;
	static constexpr StatFactory<DisplayTotalFrm, Character::Kit::postMods, Weapon::Passive::postMods, Artifact::Set::postMods, Team::postMods, preMods, StatNameFactory{}> displayTotal;
	static constexpr TalentFactory<TotalTalentsFrm, Character::Kit::talents, Character::instanceTalents, Weapon::Passive::talents, Artifact::Set::talents, Team::talents> totalTalents;
	static constexpr EnemyFactory<TotalEnemyFrm, Team::enemy, Team::resonancesEnemy, Enemy::instanceStats, EnemyNameFactory{}> totalEnemy;
}// namespace Modifiers