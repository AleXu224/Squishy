#pragma once

#include "formula/node.hpp"
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
		std::vector<Formula::Node> modifiers = {};
		std::vector<Formula::Node> totalModifiers = {};

		[[nodiscard]] inline V get(const T &statSheet) const {
#ifndef NDEBUG
			if (isRunning) {
				std::println("WARNING: recursion while computing stat, returning 0.");
				std::println("This happens because one of the modifiers requires itself through another stat");
				return 0;
			}
			isRunning = true;
#endif

			V ret = value;
			for (const auto &modifier: modifiers) ret += modifier.eval(statSheet);

#ifndef NDEBUG
			isRunning = false;
#endif

			return ret;
		}

		[[nodiscard]] inline V getTotal(const T &statSheet) const {
			V ret = value;
			for (const auto &modifier: modifiers) ret += modifier.eval(statSheet);
			for (const auto &modifier: totalModifiers) ret += modifier.eval(statSheet);
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

		static consteval auto getMembers() {
			return std::array{
				&SkillValue::DMG,
				&SkillValue::additiveDMG,
				&SkillValue::multiplicativeDMG,
				&SkillValue::critRate,
				&SkillValue::critDMG,
			};
		}

		[[nodiscard]] static constexpr bool isPercetange(SV SSV::*member) {
			if (member == &SSV::DMG) return true;
			if (member == &SSV::additiveDMG) return false;
			if (member == &SSV::multiplicativeDMG) return true;
			if (member == &SSV::critRate) return true;
			if (member == &SSV::critDMG) return true;
			return false;
		}
	};

	inline void addModifier(SV &stat, const Formula::Node &modifier) {
		stat.modifiers.emplace_back(modifier);
	}
}// namespace Stats