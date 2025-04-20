#pragma once

#include "constantOrValue.hpp"
#include "formula/formulaContext.hpp"
#include "intermediary.hpp"


namespace Formula::Compiled {
	template<BoolFormula T, class V>
	struct Requires {
		T cond;
		V ret;

		using RetType = FormulaType<V>;
		[[nodiscard]] RetType eval(const Formula::Context &context) const {
			bool condVal = cond.eval(context);
			if (!condVal) return RetType{};
			return ret.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return cond.isConstant() && ret.isConstant();
		}
	};

	[[nodiscard]] auto RequiresMaker(const Formula::Context &context, const BoolFormula auto &cond, const auto &ret) {
		if constexpr (std::is_same_v<std::remove_cvref_t<decltype(cond)>, ConstantBool>) {
			using RetType = Constant<std::remove_cvref_t<decltype(ret.eval(std::declval<const Formula::Context &>()))>>;
			if (cond.value)
				return RetType(ret.eval(context));
			else
				return RetType();
		} else {
			return Requires{
				.index = cond,
				.ret = ret,
			};
		}
	}

	template<BoolFormula T, FormulaLike V, FormulaLike U>
	struct IfElse {
		T requirement;
		V trueVal;
		U elseVal;

		using RetType = decltype(std::declval<V>().eval(std::declval<const Context &>()));
		using RetType2 = decltype(std::declval<U>().eval(std::declval<const Context &>()));
		static_assert(std::is_same_v<RetType, RetType2>, "Both formulas need to return the same type");

		[[nodiscard]] RetType eval(const Context &context) const {
			if (requirement.eval(context))
				return trueVal.eval(context);

			return elseVal.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return requirement.isConstant() && trueVal.isConstant() && elseVal.isConstant();
		}
	};

	[[nodiscard]] auto IfElseMaker(const BoolFormula auto &requirement, const FormulaLike auto &trueVal, const FormulaLike auto &elseVal) {
		if constexpr (ConstantFormula<std::remove_cvref_t<decltype(requirement)>>) {
			if constexpr (std::is_same_v<std::remove_cvref_t<decltype(trueVal)>, std::remove_cvref_t<decltype(elseVal)>>) {
				if (requirement.value) {
					return trueVal;
				} else {
					return elseVal;
				}
			} else {
				using RetType = ValueOr<std::remove_cvref_t<decltype(trueVal)>, std::remove_cvref_t<decltype(elseVal)>>;
				if (requirement.value) {
					return RetType{trueVal};
				} else {
					return RetType{elseVal};
				}
			}
		} else {
			return IfElse{requirement, trueVal, elseVal};
		}
	}
}// namespace Formula::Compiled