#pragma once

#include "agent/instance.hpp"
#include "formula/base.hpp"
#include "misc/attackSource.hpp"
#include "misc/attribute.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"


namespace Formula {
	[[nodiscard]] constexpr auto getAttribute(const Utils::JankyOptional<Misc::AttackSource> &attackSource, Utils::JankyOptional<Misc::Attribute> element, const Formula::Context &context) {
		if (element.has_value()) return element.value();
		if (!attackSource.has_value()) return element.value_or(Misc::Attribute::physical);
		return context.source.stats.base.attribute;
	}

	struct AttributeCount : FormulaBase<int32_t, Type::constant> {
		::Misc::Attribute attribute;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} count {}", Utils::Stringify(attribute), eval(context));
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
			return fmt::format("{} count {}", Utils::Stringify(attribute), eval(context));
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
			return fmt::format("Team same attribute count {}", eval(context));
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
			return fmt::format("Team other attribute count {}", eval(context));
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
			return fmt::format("Is agent {} ({})", Utils::Stringify(attribute), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.stats.base.attribute == attribute;
		}
	};

	// Used when needing to check the attribute of the agent that is to receive a team buff.
	// If this is not used in a team buff context then the result can be incorrect, use `IsAgentAttribute` instead.
	struct IsTargetAgentAttribute : FormulaBase<bool, Type::constant> {
		Misc::Attribute attribute;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is agent {} ({})", Utils::Stringify(attribute), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.prevSource.stats.base.attribute == attribute;
		}
	};
}// namespace Formula