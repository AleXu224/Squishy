#pragma once

#include "functional"
#include "print"
#include "stats/helpers.hpp"
#include "stats/stat.hpp"
#include "vector"


namespace Stats {
	template<class T, class V>
	struct Value {
#ifndef NDEBUG
		mutable bool isRunning = false;
#endif

		V value = 0;
		std::vector<std::function<V(const T &statSheet)>> modifiers = {};
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

#ifndef NDEBUG
			isRunning = false;
#endif

			return ret;
		}

		V getTotal(const T &statSheet) const {
			V ret = value;
			for (const auto &modifier: modifiers) ret += modifier(statSheet);
			for (const auto &modifier: totalModifiers) ret += modifier(statSheet);
			return ret;
		}


		static consteval auto Multiplier(const Stat &stat, V value)
			requires(SheetLike<T>)
		{
			return [stat, value](const T &statSheet) -> V {
				return statSheet.sheet.fromStat(stat).getTotal(statSheet) * value;
			};
		}

		static consteval auto Multiplier(const Stat &stat, V value) {
			return [stat, value](const T &statSheet) -> V {
				return statSheet.character.sheet.fromStat(stat).getTotal(statSheet) * value;
			};
		}

		static consteval auto Constant(V value) {
			return [value](const T &) {
				return value;
			};
		}
	};

	template<class T, class V>
	struct SkillValue {
		Value<T, V> DMG{};
		Value<T, V> additiveDMG{};
		Value<T, V> multiplicativeDMG{};
		Value<T, V> critRate{};
		Value<T, V> critDMG{};

		static inline auto getMembers() {
			return std::array{
				&SkillValue::DMG,
				&SkillValue::additiveDMG,
				&SkillValue::multiplicativeDMG,
				&SkillValue::critRate,
				&SkillValue::critDMG,
			};
		}
	};

	inline void addModifier(SV &stat, const std::function<float(const Stats::Sheet &)> modifier) {
		stat.modifiers.emplace_back(modifier);
	}
}// namespace Stats