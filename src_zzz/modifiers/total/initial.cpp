#include "initial.hpp"

#include "formula/operators.hpp"
#include "modifiers/agent/instance.hpp"
#include "modifiers/agent/kit.hpp"
#include "modifiers/disc/set.hpp"
#include "modifiers/disc/subStats.hpp"
#include "modifiers/engine/instance.hpp"
#include "modifiers/engine/passive.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/team/team.hpp"


namespace Modifiers {
	using namespace Formula::Operators;
	struct PreModFrm : Formula::FormulaBase<float> {
		Formula::FloatNode agentKit;
		Formula::FloatNode agentInstance;
		Formula::FloatNode enginePassive;
		Formula::FloatNode engineInstance;
		Formula::FloatNode discSet;
		Formula::FloatNode discSubStats;
		Formula::FloatNode teamInitial;
		Formula::FloatNode activeInitial;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			auto ret = agentKit
					 + agentInstance
					 + enginePassive
					 + engineInstance
					 + discSet
					 + discSubStats
					 + teamInitial
					 + activeInitial;
			return ret.fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (agentKit
					+ agentInstance
					+ enginePassive
					+ engineInstance
					+ discSet
					+ discSubStats
					+ teamInitial
					+ activeInitial)
				.print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return agentKit.eval(context)
				 + agentInstance.eval(context)
				 + enginePassive.eval(context)
				 + engineInstance.eval(context)
				 + discSet.eval(context)
				 + discSubStats.eval(context)
				 + teamInitial.eval(context)
				 + activeInitial.eval(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &initial() {
		static auto ret = statFactory<Formula::FloatNode, PreModFrm>(
			Agent::Kit::initial(),
			Agent::instanceInitial(),
			Engine::Passive::initial(),
			Engine::instanceInitial(),
			Disc::Set::initial(),
			Disc::subStats(),
			Team::initial(),
			Team::activeInitial()
		);
		return ret;
	}
}// namespace Modifiers