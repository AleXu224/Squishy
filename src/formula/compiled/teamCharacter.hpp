#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"
#include <algorithm>


namespace Formula::Compiled {
	template<FloatFormula T>
	struct LunarDmgDistribution : FormulaBase<float> {
		T char1;
		T char2;
		T char3;
		T char4;

		[[nodiscard]] float eval(const Formula::Context &context) const {
			std::array<float, 4> vals{
				char1.eval(context),
				char2.eval(context),
				char3.eval(context),
				char4.eval(context),
			};

			std::sort(vals.begin(), vals.end(), std::greater<float>());

			return vals.at(0)
				 + (vals.at(1) * 0.5f)
				 + ((vals.at(2) + vals.at(3)) * (1.f / 12.f));
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("LunarDmgDist<{}, {}, {}, {}>", char1.print(), char2.print(), char3.print(), char4.print());
		}
	};
}// namespace Formula::Compiled