#pragma once

#include "formula/constant.hpp"

namespace Formula {
	template<class T_elementBonus = Constant, class T_shield_ = Constant, class T_additiveHp = Constant>
	struct ShieldModifier {
		T_elementBonus elementBonus{1.f};
		T_shield_ shield_{};
		T_additiveHp additiveHp{};
	};
}// namespace Formula