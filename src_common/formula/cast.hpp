#pragma once

#include "formula/base.hpp"

namespace Formula {
	template<class To, FormulaLike T>
	struct Cast : FormulaBase<To> {
		T formula;

		using RetType = To;

		[[nodiscard]] NodeType<RetType> fold(const Context &context, const FoldArgs &args) const {
			auto ret = formula.fold(context, args);
			auto type = ret.getType();
			if (type == Formula::Type::constant) {
				return ConstantBase<RetType>{
					.value = static_cast<RetType>(ret.getConstantValue()),
				};
			}
			return Cast<RetType, decltype(ret)>{
				.formula = ret,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			return formula.print(context, prevStep);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return static_cast<RetType>(formula.eval(context));
		}
	};
}// namespace Formula