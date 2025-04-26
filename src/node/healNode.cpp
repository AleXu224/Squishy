#include "healNode.hpp"

#include "modifiers/total/total.hpp"

namespace Node {
	using namespace Formula::Operators;
	Formula::FloatNode Heal::_getFormula(
		Formula::FloatNode formula,
		Formula::HealModifier modifier
	) {
		auto totalHb = Modifiers::total.hb + modifier.hb;
		auto totalIncHb = Modifiers::total.incHb + modifier.incHb;

		auto bonus = Formula::Constant(1.f) + totalHb + totalIncHb;

		return formula
			 * bonus;
	}
}// namespace Node