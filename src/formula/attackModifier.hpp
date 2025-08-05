#pragma once

#include "formula/constant.hpp"
#include "formula/node.hpp"

namespace Formula {
	struct Modifier {
		Formula::FloatNode DMG{Formula::Constant(0.f)};
		Formula::FloatNode additiveDMG{Formula::ConstantFlat(0.f)};
		Formula::FloatNode multiplicativeDMG{Formula::Constant(0.f)};
		Formula::FloatNode elevation{Formula::Constant(0.f)};
		Formula::FloatNode critRate{Formula::Constant(0.f)};
		Formula::FloatNode critDMG{Formula::Constant(0.f)};
	};
}// namespace Formula