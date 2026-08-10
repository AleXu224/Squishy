#pragma once

#include "formula/base.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Requirement {
	static constexpr struct impl_SelfBuff : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Self Buff", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.instanceKey == context.prevSource.instanceKey;
		}
	} selfBuff{};
	static constexpr struct impl_Passive1 : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Passive 1", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.ascension >= 1;
		}
	} passive1{};
	static constexpr struct impl_Passive2 : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Passive 2", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.ascension >= 4;
		}
	} passive2{};
	static constexpr struct impl_Passive3 : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Passive 3", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return true;
		}
	} passive3{};
	static constexpr struct impl_Constellation1 : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Constellation 1", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 1;
		}
	} constellation1{};
	static constexpr struct impl_Constellation2 : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Constellation 2", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 2;
		}
	} constellation2{};
	static constexpr struct impl_Constellation3 : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Constellation 3", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 3;
		}
	} constellation3{};
	static constexpr struct impl_Constellation4 : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Constellation 4", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 4;
		}
	} constellation4{};
	static constexpr struct impl_Constellation5 : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Constellation 5", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 5;
		}
	} constellation5{};
	static constexpr struct impl_Constellation6 : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Constellation 6", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.source.stats.sheet.constellation >= 6;
		}
	} constellation6{};
	static constexpr struct impl_AscendantGleam : Formula::FormulaBase<bool, Formula::Type::constant> {
		static void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) {
			descriptor.add("Ascendant Gleam", eval(context));
		}

		[[nodiscard]] static bool eval(const Formula::Context &context) {
			return context.team.moonsignLevel.eval(context) >= 2;
		}
	} ascendantGleam{};
}// namespace Requirement