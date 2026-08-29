#include "team.hpp"
#include "agent/instance.hpp"
#include "formula/option.hpp"
#include "formula/teamAgent.hpp"
#include "modifiers/agent/kit.hpp"
#include "modifiers/disc/set.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/engine/passive.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/skillFactory.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/team.hpp"


namespace Modifiers::Team {
	using namespace Formula::Operators;
	template<class T>
	struct Frm : Formula::FormulaBase<RetType<T>> {
		T agentStat;
		T engineStat;
		T discStat;
		SheetMemberIdentifier member;
		using Ret = RetType<T>;
		[[nodiscard]] Formula::NodeType<Ret> fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto formula = agentStat + engineStat + discStat;
			auto ret = Formula::TeamAgent{.index = 0, .formula = formula}
					 + Formula::TeamAgent{.index = 1, .formula = formula}
					 + Formula::TeamAgent{.index = 2, .formula = formula};

			return ret.fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step prevStep) const {
			auto combined = agentStat + engineStat + discStat;
			auto formula = Formula::TeamAgent{.index = 0, .formula = combined}
						 + Formula::TeamAgent{.index = 1, .formula = combined}
						 + Formula::TeamAgent{.index = 2, .formula = combined};
			// Formula::Descriptor formulaDescriptor;
			formula.print(descriptor, context, prevStep);
			// if constexpr (std::is_same_v<Ret, float>) {
			// 	descriptor.add("Team " + member.getName(), {eval(context), member.isPercentage()}, std::move(formulaDescriptor));
			// } else {
			// 	descriptor.add("Team " + member.getName(), eval(context), std::move(formulaDescriptor));
			// }
		}


		[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
			Ret total = 0;
			for (const auto &agent: context.team.agents) {
				if (!agent) continue;
				auto newContext = context.withSource(agent->state);
				total += agentStat.eval(newContext)
					   + engineStat.eval(newContext)
					   + discStat.eval(newContext);
			}
			return total;
		}
	};

	struct FrmActive : Formula::FormulaBase<float> {
		Formula::FloatNode agentStat;
		Formula::FloatNode engineStat;
		Formula::FloatNode discStat;
		SheetMemberIdentifier member;

		using Ret = float;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto activeAgent = context.team.agents.at(Formula::activeAgentIndex(context));
			if (!activeAgent || activeAgent->instanceKey != context.source.instanceKey) return Formula::ConstantFlat{};

			auto formula = agentStat + engineStat + discStat;
			auto ret = Formula::TeamAgent{.index = 0, .formula = formula}
					 + Formula::TeamAgent{.index = 1, .formula = formula}
					 + Formula::TeamAgent{.index = 2, .formula = formula};
			return ret.fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step prevStep) const {
			auto combined = agentStat + engineStat + discStat;
			auto formula = Formula::ActiveAgentFilter{
				.formula = Formula::TeamAgent{.index = 0, .formula = combined}
						 + Formula::TeamAgent{.index = 1, .formula = combined}
						 + Formula::TeamAgent{.index = 2, .formula = combined},
			};
			// Formula::Descriptor formulaDescriptor;
			formula.print(descriptor, context, prevStep);
			// descriptor.add("Team " + member.getName(), {eval(context), member.isPercentage()}, std::move(formulaDescriptor));
		}


		[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
			Ret total = 0;
			auto activeAgent = context.team.agents.at(Formula::activeAgentIndex(context));
			if (!activeAgent || activeAgent->instanceKey != context.source.instanceKey) return 0;

			for (const auto &agent: context.team.agents) {
				if (!agent) continue;
				auto newContext = context.withSource(agent->state);
				total += agentStat.eval(newContext)
					   + engineStat.eval(newContext)
					   + discStat.eval(newContext);
			}
			return total;
		}
	};

	const Stats::Sheet<Formula::FloatNode> &initial() {
		static auto ret = statFactory<Formula::FloatNode, Frm<Formula::FloatNode>>(Agent::Kit::teamInitial(), Engine::Passive::teamInitial(), Disc::Set::teamInitial(), StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &combat() {
		static auto ret = statFactory<Formula::FloatNode, Frm<Formula::FloatNode>>(Agent::Kit::teamCombat(), Engine::Passive::teamCombat(), Disc::Set::teamCombat(), StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activeInitial() {
		static auto ret = statFactory<Formula::FloatNode, FrmActive>(Agent::Kit::activeInitial(), Engine::Passive::activeInitial(), Disc::Set::activeInitial(), StatNameFactory{});
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activeCombat() {
		static auto ret = statFactory<Formula::FloatNode, FrmActive>(Agent::Kit::activeCombat(), Engine::Passive::activeCombat(), Disc::Set::activeCombat(), StatNameFactory{});
		return ret;
	}
	const Skills<Formula::IntNode> &skills() {
		static auto ret = skillFactory<Formula::IntNode, Frm<Formula::IntNode>>(Agent::Kit::teamSkills(), Engine::Passive::teamSkills(), Disc::Set::teamSkills(), SkillNameFactory{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, Frm<Formula::FloatNode>>(Agent::Kit::enemy(), Engine::Passive::enemy(), Disc::Set::enemy(), EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers::Team