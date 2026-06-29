#pragma once

#include "agent/instance.hpp"
#include "formula/base.hpp"
#include "misc/attribute.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"


namespace Formula {
	[[nodiscard]] constexpr auto getAttribute(Utils::JankyOptional<Misc::Attribute> element, const Formula::Context &context) {
		if (element.has_value()) return element.value();
		return context.source.stats.base.attribute;
	}

	struct AttributeCount : FormulaBase<int32_t, Type::constant> {
		::Misc::Attribute attribute;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("{} count {}", Utils::Stringify(attribute), eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &agent: context.team.agents) {
				if (!agent) continue;
				if (agent->state.stats.base.attribute == attribute) ret++;
			}
			return ret;
		}
	};

	struct AttributeCountOthers : FormulaBase<int32_t, Type::constant> {
		::Misc::Attribute attribute;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("{} count {}", Utils::Stringify(attribute), eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &agent: context.team.agents) {
				if (!agent || &agent->state == &context.source) continue;
				if (agent->state.stats.base.attribute == attribute) ret++;
			}
			return ret;
		}
	};


	struct SameAttributeCount : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("Team same attribute count {}", eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &agent: context.team.agents) {
				if (!agent || &agent->state == &context.source) continue;
				if (agent->state.stats.base.attribute == context.source.stats.base.attribute) ret++;
			}

			return ret;
		}
	};

	struct OtherAttributeCount : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("Team other attribute count {}", eval(context));
		}

		[[nodiscard]] int32_t eval(const Context &context) const {
			uint32_t ret = 0;
			for (const auto &agent: context.team.agents) {
				if (!agent || &agent->state == &context.source) continue;
				if (agent->state.stats.base.attribute != context.source.stats.base.attribute) ret++;
			}

			return ret;
		}
	};

	struct IsAgentAttribute : FormulaBase<bool, Type::constant> {
		Misc::Attribute attribute;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("Is agent {} ({})", Utils::Stringify(attribute), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.stats.base.attribute == attribute;
		}
	};

	struct IsAgentVortexableAttribute : FormulaBase<bool, Type::constant> {
		size_t agentIndex;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("Is agent vortexable {} ({})", agentIndex, eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			auto agent = context.team.agents.at(agentIndex);
			if (!agent) return false;
			auto attribute = agent->state.stats.base.attribute;
			switch (attribute) {
				case Misc::Attribute::fire:
				case Misc::Attribute::electric:
				case Misc::Attribute::ether:
				case Misc::Attribute::ice:
				case Misc::Attribute::physical:
					return true;
				default:
					return false;
			}
		}
	};

	// Used when needing to check the attribute of the agent that is to receive a team buff.
	// If this is not used in a team buff context then the result can be incorrect, use `IsAgentAttribute` instead.
	struct IsTargetAgentAttribute : FormulaBase<bool, Type::constant> {
		Misc::Attribute attribute;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("Is target agent {} ({})", Utils::Stringify(attribute), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.prevSource.stats.base.attribute == attribute;
		}
	};
}// namespace Formula