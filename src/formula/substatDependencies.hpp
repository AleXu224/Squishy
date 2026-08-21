#pragma once

#include "stats/stat.hpp"

#include <unordered_set>

namespace Formula {
	inline thread_local std::unordered_set<::Stat> *substatDependencyCollector = nullptr;

	inline void recordSubstatDependency(::Stat stat) {
		if (substatDependencyCollector) substatDependencyCollector->insert(stat);
	}

	struct SubstatDependencyScope {
		std::unordered_set<::Stat> dependencies;
		std::unordered_set<::Stat> *previous;

		SubstatDependencyScope() : previous(substatDependencyCollector) {
			substatDependencyCollector = &dependencies;
		}
		SubstatDependencyScope(const SubstatDependencyScope &) = delete;
		SubstatDependencyScope &operator=(const SubstatDependencyScope &) = delete;
		~SubstatDependencyScope() {
			substatDependencyCollector = previous;
		}
	};
}// namespace Formula
