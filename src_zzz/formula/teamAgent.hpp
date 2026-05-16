#pragma once

#include "agent/data.hpp"
#include "agent/instance.hpp"
#include "fmt/core.h"
#include "formula/base.hpp"
#include "formula/constant.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"


namespace Formula {
	template<FormulaLike T>
	struct TeamAgent : FormulaBase<FormulaType<T>> {
		size_t index = 0;
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			const auto &agent = context.team.agents.at(index);
			if (!agent) return ConstantBase<RetType>{.value = {}};
			auto ret = formula.fold(context.withSource(agent->state), args);
			auto type = ret.getType();
			if (type == Formula::Type::constant) {
				return ret;
			}
			return TeamAgent<decltype(ret)>{
				.index = index,
				.formula = ret,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			const auto &agent = context.team.agents.at(index);
			if (!agent) return "";
			auto &stats = agent->state;
			return fmt::format(
				"{} {}",
				stats.stats.data.name,
				formula.print(context.withSource(stats), prevStep)
			);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			const auto &agent = context.team.agents.at(index);
			if (!agent) return {};
			return formula.eval(context.withSource(agent->state));
		}
	};

	template<FormulaLike T>
	struct ActiveAgent : FormulaBase<FormulaType<T>> {
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			const auto &agent = context.team.agents.at(context.team.activeAgentIndex);
			if (!agent) return ConstantBase<RetType>{.value = {}};
			return formula.fold(context.withSource(agent->state), args);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			const auto &agent = context.team.agents.at(context.team.activeAgentIndex);
			if (!agent) return "";
			auto &stats = agent->state;
			return fmt::format(
				"{} {}",
				stats.stats.data.name,
				formula.print(context.withSource(stats), prevStep)
			);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			const auto &agent = context.team.agents.at(context.team.activeAgentIndex);
			if (!agent) return {};
			return formula.eval(context.withSource(agent->state));
		}
	};

	template<FormulaLike T>
	struct PreviousAgent : FormulaBase<FormulaType<T>> {
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			return formula.fold(context.withSource(context.prevSource), args);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			return fmt::format(
				"{} {}",
				context.prevSource.stats.data.name,
				formula.print(context.withSource(context.prevSource), prevStep)
			);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return formula.eval(context.withSource(context.prevSource));
		}
	};

	template<FormulaLike T>
	struct OriginAgent : FormulaBase<FormulaType<T>> {
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			return formula.fold(context.withSource(context.origin), args);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			return fmt::format(
				"{} {}",
				context.origin.stats.data.name,
				formula.print(context.withSource(context.origin), prevStep)
			);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return formula.eval(context.withSource(context.origin));
		}
	};

	template<ArithmeticFormula T>
	struct TeamEvalSum : FormulaBase<FormulaType<T>> {
		T formula;

		using RetType = FormulaType<T>;
		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			using namespace Formula::Operators;
			return (TeamAgent{.index = 0, .formula = formula}
					+ TeamAgent{.index = 1, .formula = formula}
					+ TeamAgent{.index = 2, .formula = formula})
				.fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step) {
			return fmt::format("{}", eval(context));
		}

		[[nodiscard]] RetType eval(const Context &context) {
			const auto &agent1 = context.team.agents.at(0);
			const auto &agent2 = context.team.agents.at(1);
			const auto &agent3 = context.team.agents.at(2);

			RetType ret = 0;
			if (agent1) ret += formula.eval(context.withSource(agent1->state));
			if (agent2) ret += formula.eval(context.withSource(agent2->state));
			if (agent3) ret += formula.eval(context.withSource(agent3->state));

			return ret;
		}
	};

	struct TeamAgentCount : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] static std::string print(const Context &context, Step) {
			return fmt::format("Team agent count {}", eval(context));
		}

		[[nodiscard]] static int32_t eval(const Context &context) {
			int32_t ret = 0;
			for (const auto &agent: context.team.agents) {
				if (agent) ret++;
			}

			return ret;
		}
	};

	struct ActiveAgentFilter : FormulaBase<float> {
		Formula::FloatNode formula;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			const auto &activeAgent = context.team.agents.at(context.team.activeAgentIndex);
			if (!activeAgent) return ConstantFlat{.value = 0};
			if (context.source.instanceKey != activeAgent->instanceKey) return ConstantFlat{.value = 0};
			return formula.fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step step) const {
			const auto &activeAgent = context.team.agents.at(context.team.activeAgentIndex);
			if (!activeAgent) return "";
			if (context.source.instanceKey != activeAgent->instanceKey) return "";
			return formula.print(context, step);
		}

		[[nodiscard]] float eval(const Context &context) const {
			const auto &activeAgent = context.team.agents.at(context.team.activeAgentIndex);
			if (!activeAgent) return {};
			if (context.source.instanceKey != activeAgent->instanceKey) return {};
			return formula.eval(context);
		}
	};
}// namespace Formula