#pragma once

#include "formula/base.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Requirement {
	static constexpr struct impl_SelfBuff : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Self Buff";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.instanceKey == context.prevSource.instanceKey;
		}
	} selfBuff{};
	static constexpr struct impl_Passive1 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Passive 1";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.ascension >= 1;
		}
	} passive1{};
	static constexpr struct impl_Passive2 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Passive 2";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.ascension >= 4;
		}
	} passive2{};
	static constexpr struct impl_Passive3 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Passive 3";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return true;
		}
	} passive3{};
	static constexpr struct impl_Constellation1 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 1";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 1;
		}
	} constellation1{};
	static constexpr struct impl_Constellation2 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 2";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 2;
		}
	} constellation2{};
	static constexpr struct impl_Constellation3 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 3";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 3;
		}
	} constellation3{};
	static constexpr struct impl_Constellation4 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 4";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 4;
		}
	} constellation4{};
	static constexpr struct impl_Constellation5 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 5";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 5;
		}
	} constellation5{};
	static constexpr struct impl_Constellation6 : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Constellation 6";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 6;
		}
	} constellation6{};
	static constexpr struct impl_AscendantGleam : Formula::FormulaBase<bool, Formula::Type::constant> {
		[[nodiscard]] static std::string print(const Formula::Context &, Formula::Step) {
			return "Ascendant Gleam";
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.team.moonsignLevel.eval(context) >= 2;
		}
	} ascendantGleam{};
}// namespace Requirement