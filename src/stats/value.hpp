#pragma once

#include "formula/node.hpp"
#include "print"
#include "span"
#include "stats/helpers.hpp"


namespace Stats {
	template<class T, size_t Count, class V>
	struct Value {
		static_assert(Count >= 2, "There must be enough space for two modifiers");
#ifndef NDEBUG
		mutable bool isRunning = false;
#endif

		float constant = 0.f;
		std::array<Formula::Node, Count> modifiers{};

		[[nodiscard]] inline V get(const T &statSheet) const {
#ifndef NDEBUG
			if (isRunning) {
				std::println("WARNING: recursion while computing stat, returning 0.");
				std::println("This happens because one of the modifiers requires itself through another stat");
				return 0;
			}
			isRunning = true;
#endif

			V ret = constant;
			for (const auto &modifier: std::span<Formula::Node, Count - 1>(modifiers.begin(), Count - 1)) ret += modifier.eval(statSheet);

#ifndef NDEBUG
			isRunning = false;
#endif

			return ret;
		}

		[[nodiscard]] inline V getTotal(const T &statSheet) const {
			V ret = constant;
			for (const auto &modifier: modifiers) {
				if (!modifier.hasValue()) continue;
				ret += modifier.eval(statSheet);
			}
			return ret;
		}
	};

	template<class T>
	struct SkillValue {
		using _Value = T;

		_Value DMG{};
		_Value additiveDMG{};
		_Value multiplicativeDMG{};
		_Value critRate{};
		_Value critDMG{};

		static consteval auto getMembers() {
			return std::array{
				&SkillValue::DMG,
				&SkillValue::additiveDMG,
				&SkillValue::multiplicativeDMG,
				&SkillValue::critRate,
				&SkillValue::critDMG,
			};
		}

		[[nodiscard]] static constexpr bool isPercetange(_Value SkillValue:: *member) {
			if (member == &SkillValue::DMG) return true;
			if (member == &SkillValue::additiveDMG) return false;
			if (member == &SkillValue::multiplicativeDMG) return true;
			if (member == &SkillValue::critRate) return true;
			if (member == &SkillValue::critDMG) return true;
			return false;
		}
	};

	template<class T, size_t Count, class V>
	inline void addModifier(Value<T, Count, V> &stat, Formula::Node &&modifier) {
		stat.modifiers[0] = std::move(modifier);
	}

	template<class T, size_t Count, class V>
	inline void addTotalModifier(Value<T, Count, V> &stat, Formula::Node &&modifier) {
		stat.modifiers[1] = std::move(modifier);
	}

	template<class T, size_t Count, class V>
	inline void addModifierArtifact(Value<T, Count, V> &stat, Formula::Node &&modifier) {
		if (!stat.modifiers[0].hasValue()) {
			stat.modifiers[0] = std::move(modifier);
		} else {
			stat.modifiers[2] = std::move(modifier);
		}
	}
}// namespace Stats