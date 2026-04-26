#include "team.hpp"
#include "agent/instance.hpp"
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
		using Ret = RetType<T>;
		[[nodiscard]] Formula::NodeType<Ret> fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto formula = agentStat + engineStat + discStat;
			auto ret = Formula::TeamAgent{.index = 0, .formula = formula}
					 + Formula::TeamAgent{.index = 1, .formula = formula}
					 + Formula::TeamAgent{.index = 2, .formula = formula};

			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			auto combined = agentStat + engineStat + discStat;
			return (Formula::TeamAgent{.index = 0, .formula = combined}
					+ Formula::TeamAgent{.index = 1, .formula = combined}
					+ Formula::TeamAgent{.index = 2, .formula = combined})
				.print(context, prevStep);
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

		using Ret = float;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto activeAgent = context.team.agents.at(context.team.activeAgentIndex);
			if (!activeAgent || activeAgent->instanceKey != context.source.instanceKey) return Formula::ConstantFlat{};

			auto formula = agentStat + engineStat + discStat;
			auto ret = Formula::TeamAgent{.index = 0, .formula = formula}
					 + Formula::TeamAgent{.index = 1, .formula = formula}
					 + Formula::TeamAgent{.index = 2, .formula = formula};
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			auto combined = agentStat + engineStat + discStat;
			return Formula::ActiveAgentFilter{
				.formula = Formula::TeamAgent{.index = 0, .formula = combined}
						 + Formula::TeamAgent{.index = 1, .formula = combined}
						 + Formula::TeamAgent{.index = 2, .formula = combined},
			}
				.print(context, prevStep);
		}


		[[nodiscard]] constexpr Ret eval(const Formula::Context &context) const {
			Ret total = 0;
			auto activeAgent = context.team.agents.at(context.team.activeAgentIndex);
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
		static auto ret = statFactory<Formula::FloatNode, Frm<Formula::FloatNode>>(Agent::Kit::teamInitial(), Engine::Passive::teamInitial(), Disc::Set::teamInitial());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &combat() {
		static auto ret = statFactory<Formula::FloatNode, Frm<Formula::FloatNode>>(Agent::Kit::teamCombat(), Engine::Passive::teamCombat(), Disc::Set::teamCombat());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activeInitial() {
		static auto ret = statFactory<Formula::FloatNode, FrmActive>(Agent::Kit::activeInitial(), Engine::Passive::activeInitial(), Disc::Set::activeInitial());
		return ret;
	}
	const Stats::Sheet<Formula::FloatNode> &activeCombat() {
		static auto ret = statFactory<Formula::FloatNode, FrmActive>(Agent::Kit::activeCombat(), Engine::Passive::activeCombat(), Disc::Set::activeCombat());
		return ret;
	}
	const Skills<Formula::IntNode> &skills() {
		static auto ret = skillFactory<Formula::IntNode, Frm<Formula::IntNode>>(Agent::Kit::teamSkills(), Engine::Passive::teamSkills(), Disc::Set::teamSkills());
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &enemy() {
		static auto ret = enemyFactory<Formula::FloatNode, Frm<Formula::FloatNode>>(Agent::Kit::enemy(), Engine::Passive::enemy(), Disc::Set::enemy());
		return ret;
	}
}// namespace Modifiers::Team