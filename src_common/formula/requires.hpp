#pragma once

#include "base.hpp"


namespace Formula {
	template<BoolFormula T, class V>
	struct Requires : FormulaBase<typename V::RetType> {
		T requirement;
		V ret;

		using RetType = decltype(std::declval<V>().eval(std::declval<const Context &>()));

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto foldReq = requirement.fold(ctx, args);
			auto foldRet = ret.fold(ctx, args);

			auto typeReq = foldReq.getType();
			if (typeReq == Type::constant) {
				auto valReq = foldReq.getConstantValue();
				if (valReq) {
					return foldRet;
				} else {
					return ConstantBase<RetType>{.value = RetType{}};
				}
			}

			return Requires<decltype(foldReq), decltype(foldRet)>{
				.requirement = foldReq,
				.ret = foldRet,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto cond = requirement.eval(context);
			return std::format("{}", cond ? ret.print(context, prevStep) : "");
			// return std::format("{}", cond ? std::format("{} ({})", requirement.print(context, Step::none), ret.print(context, Step::none)) : "");
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			if (requirement.eval(context))
				return ret.eval(context);

			return RetType{};
		}
	};

	template<BoolFormula T, FormulaLike V, FormulaLike U>
	struct IfElse : FormulaBase<typename V::RetType> {
		T requirement;
		V trueVal;
		U elseVal;

		using RetType = decltype(std::declval<V>().eval(std::declval<const Context &>()));
		using RetType2 = decltype(std::declval<U>().eval(std::declval<const Context &>()));
		static_assert(std::is_same_v<RetType, RetType2>, "Both formulas need to return the same type");

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto foldReq = requirement.fold(ctx, args);
			auto foldTrue = trueVal.fold(ctx, args);
			auto foldElse = elseVal.fold(ctx, args);

			auto typeReq = foldReq.getType();
			if (typeReq == Type::constant) {
				auto valReq = foldReq.getConstantValue();
				if (valReq) {
					return foldTrue;
				} else {
					return foldElse;
				}
			}

			return IfElse<decltype(foldReq), decltype(foldTrue), decltype(foldElse)>{
				.requirement = foldReq,
				.trueVal = foldTrue,
				.elseVal = foldElse,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto cond = requirement.eval(context);
			return std::format("{}", cond ? trueVal.print(context, prevStep) : elseVal.print(context, prevStep));
			// return std::format("{} ({})", requirement.print(context, Step::none), cond ? trueVal.print(context, Step::none) : elseVal.print(context, Step::none));
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			if (requirement.eval(context))
				return trueVal.eval(context);

			return elseVal.eval(context);
		}
	};
}// namespace Formula