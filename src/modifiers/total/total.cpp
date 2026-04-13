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
	struct TotalFrm : Formula::FormulaBase<float> {
		Formula::FloatNode characterKitStat;
		Formula::FloatNode weaponPassiveStat;
		Formula::FloatNode artifactSetStat;
		Formula::FloatNode teamPostStat;
		Formula::FloatNode activePostStat;
		Formula::FloatNode preModStat;
		SheetMemberIdentifier name;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = characterKitStat
					 + weaponPassiveStat
					 + artifactSetStat
					 + teamPostStat
					 + activePostStat
					 + preModStat;
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(name.getName(), eval(context), name.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			auto characterKit = characterKitStat.eval(context);
			auto weaponPassive = weaponPassiveStat.eval(context);
			auto artifactSet = artifactSetStat.eval(context);
			auto teamPost = teamPostStat.eval(context);
			auto activePost = activePostStat.eval(context);
			auto preMod = preModStat.eval(context);
			return characterKit
				 + weaponPassive
				 + artifactSet
				 + teamPost
				 + activePost
				 + preMod;
		}
	};
	struct TeamTotalFrm : Formula::FormulaBase<float> {
		Formula::FloatNode teamPreStat;
		Formula::FloatNode teamPostStat;
		Formula::FloatNode activePreStat;
		Formula::FloatNode activePostStat;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = teamPreStat
					 + teamPostStat
					 + activePreStat
					 + activePostStat;
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (teamPreStat + teamPostStat + activePreStat + activePostStat).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			auto newContext = context.withSource(context.active);
			return teamPreStat.eval(newContext)
				 + teamPostStat.eval(newContext)
				 + activePreStat.eval(newContext)
				 + activePostStat.eval(newContext);
		}
	};
	struct DisplayTotalFrm : Formula::FormulaBase<float> {
		Formula::FloatNode characterKitStat;
		Formula::FloatNode weaponPassiveStat;
		Formula::FloatNode artifactSetStat;
		Formula::FloatNode teamPostStat;
		Formula::FloatNode preModStat;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = characterKitStat
					 + weaponPassiveStat
					 + artifactSetStat
					 + teamPostStat
					 + preModStat;
			return ret.fold(context, args);
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
	struct TotalTalentsFrm : Formula::FormulaBase<int> {
		Formula::IntNode characterKitTalent;
		Formula::IntNode characterInstanceTalent;
		Formula::IntNode weaponPassiveTalent;
		Formula::IntNode artifactSetTalent;
		Formula::IntNode teamTalent;
		[[nodiscard]] Formula::IntNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = characterKitTalent
					 + characterInstanceTalent
					 + weaponPassiveTalent
					 + artifactSetTalent
					 + teamTalent;
			return ret.fold(context, args);
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
	struct TotalEnemyFrm : Formula::FormulaBase<float> {
		Formula::FloatNode teamStat;
		Formula::FloatNode teamResonanceStat;
		Formula::FloatNode instanceStat;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = teamStat
					 + teamResonanceStat
					 + instanceStat;
			return ret.fold(context, args);
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
		static auto ret = statFactory<Formula::FloatNode, TotalFrm>(Character::Kit::postMods(), Weapon::Passive::postMods(), Artifact::Set::postMods(), Team::postMods(), Team::activePostMods(), preMods(), StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &totalTeam() {
		static auto ret = statFactory<Formula::FloatNode, TeamTotalFrm>(Team::preMods(), Team::postMods(), Team::activePreMods(), Team::activePostMods(), StatNameFactory{});
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