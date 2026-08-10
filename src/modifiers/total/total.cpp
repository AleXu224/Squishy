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

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			auto ret = characterKitStat
					 + weaponPassiveStat
					 + artifactSetStat
					 + teamPostStat
					 + activePostStat
					 + preModStat;
			Formula::Descriptor formulaDescriptor;
			ret.print(formulaDescriptor, context, Formula::Step::none);
			descriptor.add("Total " + name.getName(), {eval(context), name.isPercentage()}, std::move(formulaDescriptor));
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

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step prevStep) const {
			auto formula = teamPreStat + teamPostStat + activePreStat + activePostStat;
			// Formula::Descriptor formulaDescriptor;
			formula.print(descriptor, context, prevStep);
			// descriptor.add("Team Total " + name.getName(), {eval(context), name.isPercentage()}, std::move(formulaDescriptor));
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return teamPreStat.eval(context)
				 + teamPostStat.eval(context)
				 + activePreStat.eval(context)
				 + activePostStat.eval(context);
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

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step prevStep) const {
			(characterKitStat + weaponPassiveStat + artifactSetStat + teamPostStat + preModStat).print(descriptor, context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return characterKitStat.eval(context)
				 + weaponPassiveStat.eval(context)
				 + artifactSetStat.eval(context)
				 + teamPostStat.eval(context)
				 + preModStat.eval(context);
		}
	};
	struct TotalTalentsFrm : Formula::FormulaBase<int> {
		Formula::IntNode characterKitTalent;
		Formula::IntNode characterInstanceTalent;
		Formula::IntNode weaponPassiveTalent;
		Formula::IntNode artifactSetTalent;
		Formula::IntNode teamTalent;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::IntNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = characterKitTalent
					 + characterInstanceTalent
					 + weaponPassiveTalent
					 + artifactSetTalent
					 + teamTalent;
			return ret.fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step prevStep) const {
			auto formula = characterKitTalent + characterInstanceTalent + weaponPassiveTalent + artifactSetTalent + teamTalent;
			Formula::Descriptor formulaDescriptor;
			formula.print(formulaDescriptor, context, prevStep);
			descriptor.add("Total " + name.getName(), formula.eval(context), std::move(formulaDescriptor));
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

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			auto ret = teamStat
					 + teamResonanceStat
					 + instanceStat;
			Formula::Descriptor formulaDescriptor;
			ret.print(formulaDescriptor, context, Formula::Step::none);
			descriptor.add("Total Enemy " + name.getName(), {eval(context), name.isPercentage()}, std::move(formulaDescriptor));
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
		static auto ret = talentFactory<Formula::IntNode, TotalTalentsFrm>(Character::Kit::talents(), Character::instanceTalents(), Weapon::Passive::talents(), Artifact::Set::talents(), Team::talents(), TalentNameFactory{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &totalEnemy() {
		static auto ret = enemyFactory<Formula::FloatNode, TotalEnemyFrm>(Team::enemy(), Team::resonancesEnemy(), Enemy::instanceStats(), EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers