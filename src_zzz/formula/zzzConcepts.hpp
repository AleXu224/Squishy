#pragma once

#include "formula/context.hpp"// IWYU pragma: keep


namespace Formula {
	template<class T>
	concept AdditionalFormulaLike = false;
	template<class T>
	concept AdditionalArithmeticFormula = false;
}// namespace Formula