#pragma once

#include "cassert"
#include "formula/formulaContext.hpp"
#include "formula/node.hpp"
#include "ranges"

namespace Stats {
	template<class T, size_t Count>
	struct Value {
		// #ifndef NDEBUG
		// 		mutable bool isRunning = false;
		// #endif

		T constant = T{};
		std::array<Formula::NodeType<T>, Count> modifiers{};

		struct Compiled {
			T constant;
			std::array<Formula::Compiled::NodeType<T>, Count> vals{};

			[[nodiscard]] T eval(const Formula::Context &context) const {
				T ret = constant;
				for (const auto &val: vals) {
					if (!val.hasValue()) continue;
					ret += val.eval(context);
				}
				return ret;
			}

			[[nodiscard]] bool isConstant() const {
				for (const auto &val: vals) {
					if (!val.hasValue()) continue;
					if (!val.isConstant()) return false;
				}
				return true;
			}
		};

		[[nodiscard]] inline Compiled compile(const Formula::Context &context) const {
			Compiled ret{
				.constant = constant,
				.vals{},
			};

			for (auto [retEntry, modifier]: std::views::zip(ret.vals, modifiers)) {
				if (!modifier.hasValue()) continue;
				retEntry = modifier.compile(context);
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

		[[nodiscard]] static constexpr bool isPercetange(_Value SkillValue::*member) {
			if (member == &SkillValue::DMG) return true;
			if (member == &SkillValue::additiveDMG) return false;
			if (member == &SkillValue::multiplicativeDMG) return true;
			if (member == &SkillValue::critRate) return true;
			if (member == &SkillValue::critDMG) return true;
			return false;
		}
	};
}// namespace Stats