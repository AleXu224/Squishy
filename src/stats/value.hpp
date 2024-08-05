#pragma once

#include "functional"
#include "print"
#include "vector"


namespace Stats {
	template<class T, class V = float>
	struct Value {
#ifndef NDEBUG
		mutable bool isRunning = false;
#endif

		V value = 0;
		std::vector<std::function<V(const T &statSheet)>> modifiers = {};
		std::vector<std::function<V(const T &statSheet)>> artifactModifiers = {};
		std::vector<std::function<V(const T &statSheet)>> totalModifiers = {};

		V get(const T &statSheet) const {
#ifndef NDEBUG
			if (isRunning) {
				std::println("WARNING: recursion while computing stat, returning 0.");
				std::println("This happens because one of the modifiers requires itself through another stat");
				return 0;
			}
			isRunning = true;
#endif

			V ret = value;
			for (const auto &modifier: modifiers) ret += modifier(statSheet);
			for (const auto &modifier: artifactModifiers) ret += modifier(statSheet);

#ifndef NDEBUG
			isRunning = false;
#endif

			return ret;
		}

		V getTotal(const T &statSheet) const {
			V ret = value;
			for (const auto &modifier: modifiers) ret += modifier(statSheet);
			for (const auto &modifier: artifactModifiers) ret += modifier(statSheet);
			for (const auto &modifier: totalModifiers) ret += modifier(statSheet);
			return ret;
		}
	};
}// namespace Stats