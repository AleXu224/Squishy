#pragma once

#include "cassert"
#include "formula/compiled/constant.hpp"
#include "formula/compiled/operators.hpp"// IWYU pragma: keep
#include "formula/formulaContext.hpp"
#include "formula/node.hpp"


namespace Stats {
	template<class T, size_t Count>
	struct Value {
		// #ifndef NDEBUG
		// 		mutable bool isRunning = false;
		// #endif

		T constant = T{};
		std::array<Formula::NodeType<T>, Count> modifiers{};

		[[nodiscard]] inline Formula::Compiled::NodeType<T> compile(const Formula::Context &context) const {
			using namespace Formula::Compiled::Operators;
			Formula::Compiled::NodeType<T> ret = Formula::Compiled::Constant<T>{.value = constant};

			for (const auto &modifier: modifiers) {
				if (!modifier.hasValue()) continue;
				ret = ret + modifier.compile(context);
			}

			return ret;
		}

		[[nodiscard]] inline T get(const Formula::Context &context) const {
			// #ifndef NDEBUG
			// 			if (isRunning) {
			// 				std::println("WARNING: recursion while computing stat, returning 0.");
			// 				std::println("This happens because one of the modifiers requires itself through another stat");
			// 				return 0;
			// 			}
			// 			isRunning = true;
			// #endif

			T ret = constant;
			for (const auto &modifier: modifiers) {
				if (!modifier.hasValue()) continue;
				ret += modifier.eval(context);
			}

			// #ifndef NDEBUG
			// 			isRunning = false;
			// #endif

			return ret;
		}
	};

	template<class T>
	struct SkillValue {
		using _Value = T;

		_Value DMG{};
		_Value additiveDMG{};
		_Value multiplicativeDMG{};
		_Value elevation{};
		_Value critRate{};
		_Value critDMG{};

		static consteval auto getMembers() {
			return std::array{
				&SkillValue::DMG,
				&SkillValue::additiveDMG,
				&SkillValue::multiplicativeDMG,
				&SkillValue::elevation,
				&SkillValue::critRate,
				&SkillValue::critDMG,
			};
		}

		[[nodiscard]] static constexpr bool isPercetange(_Value SkillValue::*member) {
			if (member == &SkillValue::DMG) return true;
			if (member == &SkillValue::additiveDMG) return false;
			if (member == &SkillValue::multiplicativeDMG) return true;
			if (member == &SkillValue::elevation) return true;
			if (member == &SkillValue::critRate) return true;
			if (member == &SkillValue::critDMG) return true;
			return false;
		}
	};
}// namespace Stats