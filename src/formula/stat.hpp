#pragma once

#include "formulaContext.hpp"
#include "modifiers/total/total.hpp"
#include "stats/stat.hpp"
#include "step.hpp"

namespace Formula {
	struct Stat {
		::Stat stat;

		[[nodiscard]] auto compile(const Context &context) const {
			return Stats::fromStat(Modifiers::total(), stat).compile(context);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			return Stats::fromStat(Modifiers::total(), stat).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return Stats::fromStat(Modifiers::total(), stat).eval(context);
		}
	};
}// namespace Formula