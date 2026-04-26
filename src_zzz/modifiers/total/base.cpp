#include "base.hpp"

#include "formula/operators.hpp"
#include "modifiers/agent/instance.hpp"
#include "modifiers/agent/kit.hpp"
#include "modifiers/disc/set.hpp"
#include "modifiers/engine/instance.hpp"
#include "modifiers/engine/passive.hpp"
#include "modifiers/statFactory.hpp"


namespace Modifiers {
	using namespace Formula::Operators;
	struct BaseFrm : Formula::FormulaBase<float> {
		Formula::FloatNode agentKit;
		Formula::FloatNode agentInstance;
		Formula::FloatNode enginePassive;
		Formula::FloatNode engineInstance;
		Formula::FloatNode discSet;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = agentKit
					 + agentInstance
					 + enginePassive
					 + engineInstance
					 + discSet;
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (agentKit
					+ agentInstance
					+ enginePassive
					+ engineInstance
					+ discSet)
				.print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return agentKit.eval(context)
				 + agentInstance.eval(context)
				 + enginePassive.eval(context)
				 + engineInstance.eval(context)
				 + discSet.eval(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &base() {
		static auto ret = statFactory<Formula::FloatNode, BaseFrm>(
			Agent::Kit::base(),
			Agent::instanceBase(),
			Engine::Passive::base(),
			Engine::instanceBase(),
			Disc::Set::base()
		);
		return ret;
	}
}// namespace Modifiers