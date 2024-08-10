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
			: print([printFn = t.print](const Stats::Sheet &stats) {
				  return printFn(stats, Step::none);
			  }),
			  eval(t.eval) {
		}
	};
}// namespace Formula