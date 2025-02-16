#pragma once

#include "formula/constant.hpp"

namespace Formula {
	template<class T_hb = Constant, class T_incHb = Constant>
	struct HealModifier {
		T_hb hb{};
		T_incHb incHb{};
	};
}// namespace Formula