#pragma once

#include "formula/base.hpp"
#include "reaction/reaction.hpp"

namespace Formula {
	struct AmplifyingMultiplier : FormulaBase<float> {
		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			switch (context.reaction.index()) {
				case 0:
					return ConstantFlat{.value = 1.f};
				case 1:
					return std::get<1>(context.reaction)->formula.fold(context, args);
				case 2:
					return ConstantFlat{.value = 1.f};
			}
			std::unreachable();
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return Percentage("Reaction Multiplier", eval(context), true);
		}

		[[nodiscard]] static constexpr float eval(const Context &context) {
			switch (context.reaction.index()) {
				case 0:
					return 1.f;
				case 1:
					return std::get<1>(context.reaction)->formula.eval(context);
				case 2:
					return 1.f;
			}
			std::unreachable();
		}
	};
	struct AdditiveMultiplier : FormulaBase<float> {
		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			switch (context.reaction.index()) {
				case 0:
					return ConstantFlat{.value = 0.f};
				case 1:
					return ConstantFlat{.value = 0.f};
				case 2:
					return std::get<2>(context.reaction)->formula.fold(context, args);
			}
			std::unreachable();
		}

		[[nodiscard]] static std::string print(const Context &context, Step) {
			return Percentage("Reaction Bonus", eval(context), true);
		}

		[[nodiscard]] static constexpr float eval(const Context &context) {
			switch (context.reaction.index()) {
				case 0:
					return 0.f;
				case 1:
					return 0.f;
				case 2:
					return std::get<2>(context.reaction)->formula.eval(context);
			}
			std::unreachable();
		}
	};
}// namespace Formula