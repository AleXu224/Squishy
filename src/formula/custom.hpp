#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<class T>
	concept EvalFuncLike = requires(T t) {
		{ t(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>()) } -> std::same_as<float>;
	};

	template<EvalFuncLike T>
	struct Custom {
		T func;
		bool isPercentage = false;

		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step) const {
			return fmt::format("{:2f}{}", func(source, target, team) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return func(source, target, team);
		}
	};
}// namespace Formula