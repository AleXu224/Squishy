#pragma once

#include "formula/constant.hpp"
#include "formula/node.hpp"

namespace Formula {
	struct HealModifier {
		FloatNode hb{Constant(0.f)};
		FloatNode incHb{Constant(0.f)};
	};
}// namespace Formula