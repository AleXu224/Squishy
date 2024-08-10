#pragma once

#include "step.hpp"
#include "utility"
#include <string>

namespace Stats {
	struct Sheet;
}

namespace Formula {
	template<class T, class U>
	struct Intermediary {
		T print;
		U eval;
	};

	template<class T>
	concept IntermediaryLike = requires(T t) {
		{ std::decay_t<T>::print(std::declval<const Stats::Sheet &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ std::decay_t<T>::eval(std::declval<const Stats::Sheet &>()) } -> std::same_as<float>;
	};
}// namespace Formula