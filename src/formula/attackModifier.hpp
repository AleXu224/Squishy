#pragma once

#include "formula/constant.hpp"

namespace Formula {
	template<class T_DMG = Constant, class T_additiveDMG = Constant, class T_multiplicativeDMG = Constant, class T_critRate = Constant, class T_critDMG = Constant>
	struct Modifier {
		T_DMG DMG{};
		T_additiveDMG additiveDMG{};
		T_multiplicativeDMG multiplicativeDMG{};
		T_critRate critRate{};
		T_critDMG critDMG{};
	};
}// namespace Formula