#include "displayStats.hpp"
#include "formula/operators.hpp"// IWYU pragma: keep
#include "modifiers/engine/instance.hpp"
#include "modifiers/statFactory.hpp"


namespace Modifiers::Engine {
	using namespace Formula::Operators;
	struct DisplayFrm : Formula::FormulaBase<float> {
		Formula::FloatNode engineInstanceBase;
		Formula::FloatNode engineInstanceInitial;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return (engineInstanceBase + engineInstanceInitial).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (engineInstanceBase + engineInstanceInitial).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return engineInstanceBase.eval(context) + engineInstanceInitial.eval(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &displayStats() {
		static auto ret = statFactory<Formula::FloatNode, DisplayFrm>(Engine::instanceBase(), Engine::instanceInitial());
		return ret;
	}
}// namespace Modifiers::Engine