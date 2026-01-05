#include "displayStats.hpp"
#include "formula/operators.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/weapon/instance.hpp"
#include "modifiers/weapon/passive.hpp"


namespace Modifiers::Weapon {
	using namespace Formula::Operators;
	using namespace Formula::Compiled::Operators;
	struct DisplayFrm {
		Formula::FloatNode weaponPassivePre;
		Formula::FloatNode weaponPassivePost;
		Formula::FloatNode weaponInstance;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return weaponPassivePre.compile(context)
				 + weaponPassivePost.compile(context)
				 + weaponInstance.compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (weaponPassivePre + weaponPassivePost + weaponInstance).print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return weaponPassivePre.eval(context)
				 + weaponPassivePost.eval(context)
				 + weaponInstance.eval(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &displayStats() {
		static auto ret = statFactory<Formula::FloatNode, DisplayFrm>(Weapon::Passive::preMods(), Weapon::Passive::postMods(), Weapon::instanceStats());
		return ret;
	}
}// namespace Modifiers::Weapon