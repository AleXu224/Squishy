#include "polarityDisorder.hpp"
#include "formula/anomaly.hpp"
#include "formula/operators.hpp"
#include "formula/teamAgent.hpp"
#include "misc/attribute.hpp"


namespace Node {
	using namespace Formula::Operators;

	Formula::FloatNode PolarityDisorder::_getFormula(
		const Formula::NodeType<Misc::Attribute> &attribute,
		const Formula::FloatNode &multiplier,
		size_t index,
		const Formula::Modifier &modifier
	) {
		auto formula = Formula::TeamAgent{
			.index = index,
			.formula = Formula::AnomalyDisorderFromAttribute{
				.attribute = attribute,
			},
		};

		return multiplier
			 * formula;
	}

}// namespace Node