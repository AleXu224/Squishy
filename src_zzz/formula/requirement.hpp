#pragma once

#include "formula/base.hpp"
#include "formula/option.hpp"
#include "stats/loadout.hpp"

namespace Requirement {
	static constexpr struct impl_SelfBuff : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Self Buff";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.instanceKey == context.prevSource.instanceKey;
		}
	} selfBuff{};
	static constexpr struct impl_Mindscape1 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Mindscape 1";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.mindscape >= 1;
		}
	} mindscape1{};
	static constexpr struct impl_Mindscape2 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Mindscape 2";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.mindscape >= 2;
		}
	} mindscape2{};
	static constexpr struct impl_Mindscape3 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Mindscape 3";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.mindscape >= 3;
		}
	} mindscape3{};
	static constexpr struct impl_Mindscape4 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Mindscape 4";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.mindscape >= 4;
		}
	} mindscape4{};
	static constexpr struct impl_Mindscape5 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Mindscape 5";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.mindscape >= 5;
		}
	} mindscape5{};
	static constexpr struct impl_Mindscape6 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Mindscape 6";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.mindscape >= 6;
		}
	} mindscape6{};

	static constexpr auto enemyStunned = Formula::impl_IsActivePassive{.name = "enemyStunned"};
}// namespace Requirement