#pragma once

#include "step.hpp"
#include "utility"
#include <string>

namespace Stats {
	struct Loadout;
}

namespace Formula {
	template<class T>
	concept IntermediaryLike = requires(T t) {
		{ t.print(std::declval<const Stats::Loadout &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ t.eval(std::declval<const Stats::Loadout &>()) } -> std::same_as<float>;
	};
}// namespace Formula