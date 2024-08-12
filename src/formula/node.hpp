#pragma once

#include "functional"
#include "string"

#include "intermediary.hpp"

namespace Stats {
	struct Sheet;
}

namespace Formula {
	struct Node {
		std::function<std::string(const Stats::Sheet &stats)> print;
		std::function<float(const Stats::Sheet &stats)> eval;

		template<IntermediaryLike T>
		Node(const T &t)
			: print([t](const Stats::Sheet &stats) {
				  return t.print(stats, Step::none);
			  }),
			  eval([t](const Stats::Sheet &stats) {
				  return t.eval(stats);
			  }) {
		}
	};
}// namespace Formula