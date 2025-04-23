#pragma once

#include "constant.hpp"
#include "formula/formulaContext.hpp"
#include "intermediary.hpp"


namespace Formula::Compiled {
	template<BoolFormula T, class V>
	struct Requires : FormulaBase<typename V::Type> {
		T cond;
		V ret;

		using RetType = FormulaType<V>;
		[[nodiscard]] RetType eval(const Formula::Context &context) const {
			bool condVal = cond.eval(context);
			if (!condVal) return RetType{};
			return ret.eval(context);
		}
	};

	[[nodiscard]] auto RequiresMaker(const BoolFormula auto &cond, const auto &ret) {
		auto type1 = cond.getType();

		if (type1 == Type::constant) {
			auto val1 = cond.getConstantValue();
			if (val1)
				return ret;
			else
				return Constant<FormulaType<decltype(ret)>>{}.wrap();
		}

		return Requires{
			.cond = cond,
			.ret = ret,
		}
			.wrap();
	}

	template<BoolFormula T, FormulaLike V, FormulaLike U>
	struct IfElse : FormulaBase<typename V::Type> {
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
	};

	[[nodiscard]] auto IfElseMaker(const BoolFormula auto &requirement, const FormulaLike auto &trueVal, const FormulaLike auto &elseVal) {
		auto type1 = requirement.getType();

		if (type1 == Type::constant) {
			auto val1 = requirement.getConstantValue();

			if (val1)
				return trueVal;
			else
				return elseVal;
		}
		return IfElse{
			.requirement = requirement,
			.trueVal = trueVal,
			.elseVal = elseVal,
		}
			.wrap();
	}
}// namespace Formula::Compiled