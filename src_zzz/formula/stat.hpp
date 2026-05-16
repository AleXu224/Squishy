#pragma once

#include "formula/base.hpp"
#include "modifiers/total/total.hpp"
#include "stats/stat.hpp"

namespace Formula {
	struct Stat : FormulaBase<float> {
		::Stat stat;

		[[nodiscard]] auto fold(const Context &context, const FoldArgs &args) const {
			return Stats::fromStat(Modifiers::combat(), stat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			return Stats::fromStat(Modifiers::combat(), stat).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return Stats::fromStat(Modifiers::combat(), stat).eval(context);
		}
	};

	template<class S, AttributeFormula T>
	struct StatFromAttribute : FormulaBase<float> {
		S sheet;
		T attribute;

		[[nodiscard]] auto fold(const Context &context, const FoldArgs &args) const {
			auto foldedAttribute = attribute.fold(context, args);

			if (foldedAttribute.getType() == Type::constant) {
				return Stats::fromAttribute(sheet, foldedAttribute.eval(context)).fold(context, args);
			}

			return StatFromAttribute<S, decltype(foldedAttribute)>{
				.sheet = sheet,
				.attribute = foldedAttribute,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto attribute = this->attribute.eval(context);
			return Stats::fromAttribute(sheet, attribute).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Context &context) const {
			auto attribute = this->attribute.eval(context);
			return Stats::fromAttribute(sheet, attribute).eval(context);
		}
	};
}// namespace Formula