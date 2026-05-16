#pragma once

#include "agent/data.hpp"
#include "agent/instance.hpp"
#include "formula/base.hpp"
#include "formula/clamp.hpp"
#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"


namespace Formula {
	struct AgentLevel : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Agent level {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.stats.sheet.level;
		}
	};

	struct AgentMindscape : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Agent mindscape {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.stats.sheet.mindscape;
		}
	};

	template<LevelableSkill skill>
	struct AgentSkill : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} lvl {}", Utils::Stringify(skill), eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return Modifiers::skills().fromSkill(skill).eval(context) + 1;
		}
	};

	struct AgentPromotion : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Agent promotion {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.stats.sheet.promotion;
		}
	};

	struct IsAgentSpecialty : FormulaBase<bool, Type::constant> {
		Misc::Specialty specialty;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is agent {} ({})", Utils::Stringify(specialty), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.stats.base.specialty == specialty;
		}
	};

	struct SpecialtyCountOthers : FormulaBase<int32_t, Type::constant> {
		::Misc::Specialty specialty;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} count {}", Utils::Stringify(specialty), eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &agent: context.team.agents) {
				if (!agent || &agent->state == &context.source) continue;
				if (agent->state.stats.base.specialty == specialty) ret++;
			}
			return ret;
		}
	};

	struct FactionCountOthers : FormulaBase<int32_t, Type::constant> {
		uint32_t factionId;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} faction count {}", factionId, eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &agent: context.team.agents) {
				if (!agent || &agent->state == &context.source) continue;
				if (agent->state.stats.data.factionId == factionId) ret++;
			}
			return ret;
		}
	};

	struct IsTargetAgentSpecialty : FormulaBase<bool, Type::constant> {
		Misc::Specialty specialty;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is target agent {} ({})", Utils::Stringify(specialty), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.prevSource.stats.base.specialty == specialty;
		}
	};

	struct IsAgentId : FormulaBase<bool, Type::constant> {
		uint32_t id;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is agent id {} ({})", id, eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.stats.data.key.key == id;
		}
	};

	struct IsOriginAgentId : FormulaBase<bool, Type::constant> {
		uint32_t id;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is origin agent id {} ({})", id, eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.origin.stats.data.key.key == id;
		}
	};

	struct IsSourceAgentId : FormulaBase<bool, Type::constant> {
		uint32_t id;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is source agent id {} ({})", id, eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.stats.data.key.key == id;
		}
	};


	template<FormulaLike T>
	struct MaxAgent : FormulaBase<FormulaType<T>> {
		T formula;

		using RetType = FormulaType<T>;

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			NodeType<RetType> ret = ConstantBase<RetType>{};

			for (const auto &agent: ctx.team.agents) {
				if (!agent) continue;
				ret = Max{.val1 = ret, .val2 = formula.fold(ctx.withSource(agent->state), args)};
			}

			return ret.fold(ctx, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			RetType ret{};
			Agent::Instance *maxAgent = nullptr;
			for (auto &agent: context.team.agents) {
				if (!agent) continue;
				auto val = formula.eval(context.withSource(agent->state));
				if (val > ret) {
					ret = val;
					maxAgent = agent;
				}
			}

			if (!maxAgent) return "";

			return fmt::format("Max Team {}", formula.print(context.withSource(maxAgent->state), prevStep));
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			RetType ret{};
			for (const auto &agent: context.team.agents) {
				if (!agent) continue;
				ret = std::max(ret, formula.eval(context.withSource(agent->state)));
			}
			return ret;
		}
	};

	struct AgentAttribute : FormulaBase<Misc::Attribute, Type::constant> {
		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Agent attribute {}", Utils::Stringify(eval(context)));
		}

		[[nodiscard]] Misc::Attribute eval(const Context &context) const {
			return context.source.stats.base.attribute;
		}
	};
}// namespace Formula
