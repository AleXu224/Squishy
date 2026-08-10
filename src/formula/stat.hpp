#pragma once

#include "formula/base.hpp"
#include "modifiers/total/total.hpp"
#include "stats/stat.hpp"

namespace Formula {
	struct Stat : FormulaBase<float> {
		::Stat stat;

		[[nodiscard]] auto fold(const Context &context, const FoldArgs &args) const {
			return Stats::fromStat(Modifiers::total(), stat).fold(context, args);
		}

		void print(Descriptor &descriptor, const Context &context, Step prevStep) const {
			Stats::fromStat(Modifiers::total(), stat).print(descriptor, context, prevStep);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return Stats::fromStat(Modifiers::total(), stat).eval(context);
		}
	};
}// namespace Formula