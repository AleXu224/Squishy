#pragma once

#include "formula/base.hpp"
#include "reaction/reaction.hpp"

namespace Formula {
	struct AmplifyingMultiplier : FormulaBase<float> {
		Formula::FloatNode getFormula(const Formula::Context &context) const {
			switch (context.reaction.index()) {
				case 0:
					return ConstantFlat{.value = 1.f};
				case 1:
					return std::get<1>(context.reaction)->formula;
				case 2:
					return ConstantFlat{.value = 1.f};
			}
			std::unreachable();
		}

		[[nodiscard]] FloatNode fold(const Formula::Context &context, const FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Context &context, Step) const {
			Descriptor formulaDescriptor;
			getFormula(context).print(formulaDescriptor, context, Step::none);
			descriptor.add("Reaction Multiplier", {eval(context), true}, formulaDescriptor);
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
		FloatNode getFormula(const Formula::Context &context) const {
			switch (context.reaction.index()) {
				case 0:
					return ConstantFlat{.value = 0.f};
				case 1:
					return ConstantFlat{.value = 0.f};
				case 2:
					return std::get<2>(context.reaction)->formula;
			}
			std::unreachable();
		}

		[[nodiscard]] FloatNode fold(const Formula::Context &context, const FoldArgs &args) const {
			return getFormula(context).fold(context, args);
		}

		void print(Descriptor &descriptor, const Context &context, Step) const {
			Descriptor formulaDescriptor;
			getFormula(context).print(formulaDescriptor, context, Step::none);
			descriptor.add("Additive Multiplier", {eval(context), true}, formulaDescriptor);
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