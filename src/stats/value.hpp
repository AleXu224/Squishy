#pragma once

#include "cassert"
#include "formula/node.hpp"
#include "print"
#include "stats/helpers.hpp"


namespace Stats {
	template<class P1, class P2, size_t Count>
	struct Value {
#ifndef NDEBUG
		mutable bool isRunning = false;
#endif

		float constant = 0.f;
		std::array<Formula::Node, Count> modifiers{};

		[[nodiscard]] inline float get(const P1 &statSheet, const P2 &team) const {
#ifndef NDEBUG
			if (isRunning) {
				std::println("WARNING: recursion while computing stat, returning 0.");
				std::println("This happens because one of the modifiers requires itself through another stat");
				return 0;
			}
			isRunning = true;
#endif

			float ret = constant;
			for (const auto &modifier: modifiers) {
				if (!modifier.hasValue()) continue;
				ret += modifier.eval(statSheet, team);
			}

#ifndef NDEBUG
			isRunning = false;
#endif

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
}// namespace Stats