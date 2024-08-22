#pragma once

#include "step.hpp"
#include "utility"
#include <string>

namespace Stats {
	struct Loadout;
	struct Team;
}// namespace Stats

namespace Formula {
	template<class T>
	concept IntermediaryLike = requires(T t) {
		{ t.print(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ t.eval(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>()) } -> std::same_as<float>;
	};
}// namespace Formula