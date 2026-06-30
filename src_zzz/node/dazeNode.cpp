#include "dazeNode.hpp"
#include "formula/nodes.hpp"
#include "formula/operators.hpp"
#include "misc/attribute.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;
	using T = Stats::Sheet<Formula::FloatNode>;

	[[nodiscard]] constexpr auto _getTotalEnemyDaze(
		Utils::JankyOptional<Misc::Attribute> attackAttribute,
		const Utils::JankyOptional<Misc::AttackSource> &source,
		const auto &formula
	) {
		Formula::EnemyModifier modifiers{};
		modifiers = modifiers + Modifiers::combat().all.enemy
				  + Formula::getAttributeModifier(attackAttribute)
				  + Formula::getSourceModifier(source)
				  + formula;

		return modifiers;
	}

	Formula::FloatNode Daze::_getFormula(
		const Utils::JankyOptional<Misc::Attribute> &attribute,
		const Utils::JankyOptional<Misc::AttackSource> &source,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalModifier = _getTotalEnemyDaze(attribute, source, modifier.enemy);

		auto dazeRes = 1.f - (Modifiers::enemy().dazeRes + totalModifier.dazeRes);
		auto daze = 1.f + (Modifiers::enemy().daze + totalModifier.daze);
		auto dazeTaken = 1.f + (Modifiers::enemy().dazeTaken + totalModifier.dazeTaken);

		return formula
			 * dazeRes
			 * daze
			 * dazeTaken;
	}

}// namespace Node