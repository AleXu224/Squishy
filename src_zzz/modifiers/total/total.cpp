#include "total.hpp"

#include "modifiers/agent/instance.hpp"
#include "modifiers/agent/kit.hpp"
#include "modifiers/disc/set.hpp"
#include "modifiers/enemy/instance.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/engine/passive.hpp"
#include "modifiers/skillFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/team/resonance.hpp"
#include "modifiers/team/team.hpp"


namespace Modifiers {
	using namespace Formula::Operators;
	struct TotalFrm : Formula::FormulaBase<float> {
		Formula::FloatNode agentKit;
		Formula::FloatNode agentInstance;
		Formula::FloatNode enginePassive;
		Formula::FloatNode discSet;
		Formula::FloatNode team;
		Formula::FloatNode active;
		Formula::FloatNode resonances;
		SheetMemberIdentifier name;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = agentKit
					 + agentInstance
					 + enginePassive
					 + discSet
					 + team
					 + active
					 + resonances;

			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(name.getName(), eval(context), name.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return agentKit.eval(context)
				 + agentInstance.eval(context)
				 + enginePassive.eval(context)
				 + discSet.eval(context)
				 + team.eval(context)
				 + active.eval(context)
				 + resonances.eval(context);
		}
	};
	struct TeamTotalFrm : Formula::FormulaBase<float> {
		Formula::FloatNode teamInitial;
		Formula::FloatNode teamCombat;
		Formula::FloatNode activeInitial;
		Formula::FloatNode activeCombat;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = teamInitial
					 + teamCombat
					 + activeInitial
					 + activeCombat;
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (teamInitial + teamCombat + activeInitial + activeCombat).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			auto newContext = context.withSource(context.active);
			return teamInitial.eval(newContext)
				 + teamCombat.eval(newContext)
				 + activeInitial.eval(newContext)
				 + activeCombat.eval(newContext);
		}
	};
	struct DisplayTotalFrm : Formula::FormulaBase<float> {
		Formula::FloatNode agentKitStat;
		Formula::FloatNode enginePassiveStat;
		Formula::FloatNode discSetStat;
		Formula::FloatNode teamPostStat;
		Formula::FloatNode preModStat;
		SheetMemberIdentifier name;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = agentKitStat
					 + enginePassiveStat
					 + discSetStat
					 + teamPostStat
					 + preModStat;
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (agentKitStat + enginePassiveStat + discSetStat + teamPostStat + preModStat).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			auto newContext = context.withSource(context.active);
			return agentKitStat.eval(newContext)
				 + enginePassiveStat.eval(newContext)
				 + discSetStat.eval(newContext)
				 + teamPostStat.eval(newContext)
				 + preModStat.eval(newContext);
		}
	};
	struct TotalSkillsFrm : Formula::FormulaBase<int> {
		Formula::IntNode agentKitSkill;
		Formula::IntNode agentInstanceSkill;
		Formula::IntNode enginePassiveSkill;
		Formula::IntNode discSetSkill;
		Formula::IntNode teamSkill;
		[[nodiscard]] Formula::IntNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = agentKitSkill
					 + agentInstanceSkill
					 + enginePassiveSkill
					 + discSetSkill
					 + teamSkill;
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (agentKitSkill + agentInstanceSkill + enginePassiveSkill + discSetSkill + teamSkill).print(context, prevStep);
		}

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return agentKitSkill.eval(context)
				 + agentInstanceSkill.eval(context)
				 + enginePassiveSkill.eval(context)
				 + discSetSkill.eval(context)
				 + teamSkill.eval(context);
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

	const Stats::Sheet<Formula::FloatNode> &combat() {
		static auto ret = statFactory<Formula::FloatNode, TotalFrm>(Agent::Kit::combat(), Agent::instanceCombat(), Engine::Passive::combat(), Disc::Set::combat(), Team::combat(), Team::activeCombat(), Team::resonances(), StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &team() {
		static auto ret = statFactory<Formula::FloatNode, TeamTotalFrm>(Team::initial(), Team::combat(), Team::activeInitial(), Team::activeCombat(), StatNameFactory{});
		return ret;
	}
	const Skills<Formula::IntNode> &skills() {
		static auto ret = skillFactory<Formula::IntNode, TotalSkillsFrm>(Agent::Kit::skills(), Agent::instanceSkills(), Engine::Passive::skills(), Disc::Set::skills(), Team::skills());
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, TotalEnemyFrm>(Team::enemy(), Team::resonancesEnemy(), Enemy::instanceStats(), EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers