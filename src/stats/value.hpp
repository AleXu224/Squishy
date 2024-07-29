#pragma once

#include "array"
#include "functional"
#include "print"
#include "vector"


namespace Stats {
	class CharacterSheet;

	template<class T, class V = float>
	class Value {
		mutable bool isRunning = false;

	public:
		const V value;
		std::vector<std::function<V(const T &statSheet)>> modifiers = {};
		// There can't be more than two artifact modifiers so this could be optimized by switching to an array
		//	std::vector<std::function<float(StatSheet &statSheet)>> artifactModifiers = {};

		// ~16% improvement (11 ns) by switching to an array
		// 7 entries : 5 for artifacts, 2 for sets
		std::array<std::function<V(const T &statSheet)>, 7> artifactModifiers{};
		unsigned int artifactModifiersCount = 0;

		explicit Value(const V &val) : value(val) {}

		V calculateValue(const T &statSheet) const {
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
			// Only apply artifactModifiersCount modifiers because the array is not reset upon removing old ones
			// and memory could be uninitialized
			for (unsigned int i = 0; i < artifactModifiersCount; i++) {
				ret += artifactModifiers.at(i)(statSheet);
			}

#ifndef NDEBUG
			isRunning = false;
#endif

			return ret;
		}
	};
}// namespace Stats