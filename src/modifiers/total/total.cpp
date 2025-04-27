#include "total.hpp"

#include "modifiers/artifact/set.hpp"
#include "modifiers/character/instance.hpp"
#include "modifiers/character/kit.hpp"
#include "modifiers/enemy/instance.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "modifiers/team/resonance.hpp"
#include "modifiers/team/team.hpp"
#include "modifiers/total/premod.hpp"
#include "modifiers/weapon/passive.hpp"


namespace Modifiers {
	using namespace Formula::Operators;
	struct TotalFrm {
		const Formula::FloatNode &characterKitStat;
		const Formula::FloatNode &weaponPassiveStat;
		const Formula::FloatNode &artifactSetStat;
		const Formula::FloatNode &teamPostStat;
		const Formula::FloatNode &preModStat;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return (characterKitStat + weaponPassiveStat + artifactSetStat + teamPostStat + preModStat).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(name.getName(), eval(context), name.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
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
	struct TotalActiveFrm {
		const Formula::FloatNode &characterKitStat;
		const Formula::FloatNode &weaponPassiveStat;
		const Formula::FloatNode &artifactSetStat;
		const Formula::FloatNode &teamPostStat;
		const Formula::FloatNode &preModStat;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return (characterKitStat + weaponPassiveStat + artifactSetStat + teamPostStat + preModStat).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(name.getName(), eval(context), name.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return characterKitStat.eval(context)
				 + weaponPassiveStat.eval(context)
				 + artifactSetStat.eval(context)
				 + teamPostStat.eval(context)
				 + preModStat.eval(context);
		}
	};
	struct DisplayTotalFrm {
		const Formula::FloatNode &characterKitStat;
		const Formula::FloatNode &weaponPassiveStat;
		const Formula::FloatNode &artifactSetStat;
		const Formula::FloatNode &teamPostStat;
		const Formula::FloatNode &preModStat;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return (characterKitStat + weaponPassiveStat + artifactSetStat + teamPostStat + preModStat).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (characterKitStat + weaponPassiveStat + artifactSetStat + teamPostStat + preModStat).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			auto newContext = context.withSource(context.active);
			return characterKitStat.eval(newContext)
				 + weaponPassiveStat.eval(newContext)
				 + artifactSetStat.eval(newContext)
				 + teamPostStat.eval(newContext)
				 + preModStat.eval(newContext);
		}
	};
	struct TotalTalentsFrm {
		const Formula::FloatNode &characterKitTalent;
		const Formula::FloatNode &characterInstanceTalent;
		const Formula::FloatNode &weaponPassiveTalent;
		const Formula::FloatNode &artifactSetTalent;
		const Formula::FloatNode &teamTalent;
		[[nodiscard]] Formula::Compiled::IntNode compile(const Formula::Context &context) const {
			return (characterKitTalent + characterInstanceTalent + weaponPassiveTalent + artifactSetTalent + teamTalent).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (characterKitTalent + characterInstanceTalent + weaponPassiveTalent + artifactSetTalent + teamTalent).print(context, prevStep);
		}

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return characterKitTalent.eval(context)
				 + characterInstanceTalent.eval(context)
				 + weaponPassiveTalent.eval(context)
				 + artifactSetTalent.eval(context)
				 + teamTalent.eval(context);
		}
	};
	struct TotalEnemyFrm {
		const Formula::FloatNode &teamStat;
		const Formula::FloatNode &teamResonanceStat;
		const Formula::FloatNode &instanceStat;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return (teamStat + teamResonanceStat + instanceStat).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(name.getName(), eval(context), name.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return teamStat.eval(context)
				 + teamResonanceStat.eval(context)
				 + instanceStat.eval(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &total() {
		static auto ret = statFactory<Formula::FloatNode, TotalFrm>(Character::Kit::postMods(), Weapon::Passive::postMods(), Artifact::Set::postMods(), Team::postMods(), preMods(), StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &totalActive() {
		static auto ret = statFactory<Formula::FloatNode, TotalActiveFrm>(Character::Kit::postMods(), Weapon::Passive::postMods(), Artifact::Set::postMods(), Team::postMods(), preMods(), StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &displayTotal() {
		static auto ret = statFactory<Formula::FloatNode, DisplayTotalFrm>(Character::Kit::postMods(), Weapon::Passive::postMods(), Artifact::Set::postMods(), Team::postMods(), preMods(), StatNameFactory{});
		return ret;
	}
	const Talents<Formula::IntNode> &totalTalents() {
		static auto ret = talentFactory<Formula::IntNode, TotalTalentsFrm>(Character::Kit::talents(), Character::instanceTalents(), Weapon::Passive::talents(), Artifact::Set::talents(), Team::talents());
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &totalEnemy() {
		static auto ret = enemyFactory<Formula::FloatNode, TotalEnemyFrm>(Team::enemy(), Team::resonancesEnemy(), Enemy::instanceStats(), EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers