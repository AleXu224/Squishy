#pragma once

#include "string"

#include "functional"
#include "intermediary.hpp"


namespace Stats {
	struct Sheet;
}

namespace Formula {
	struct Node {
		bool hasValue;
		std::function<std::string(const Stats::Sheet &stats)> print;
		std::function<float(const Stats::Sheet &stats)> eval;

		template<IntermediaryLike T>
		constexpr Node(const T &t)
			: hasValue(true),
			  print([t](const Stats::Sheet &stats) {
				  return t.print(stats, Step::none);
			  }),
			  eval([t](const Stats::Sheet &stats) {
				  return t.eval(stats);
			  }) {
		}

		Node()
			: hasValue(false),
			  print([](const Stats::Sheet &) -> std::string {
				  return "";
			  }),
			  eval([](const Stats::Sheet &) {
				  return 0.f;
			  }) {}
	};
}// namespace Formula