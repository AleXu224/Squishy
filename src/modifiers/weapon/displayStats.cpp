#include "displayStats.hpp"
#include "formula/operators.hpp"// IWYU pragma: keep
#include "modifiers/statFactory.hpp"
#include "modifiers/weapon/instance.hpp"


namespace Modifiers::Weapon {
	using namespace Formula::Operators;
	using namespace Formula::Compiled::Operators;
	struct DisplayFrm {
		Formula::FloatNode weaponInstance;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return weaponInstance.compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return weaponInstance.print(context, prevStep);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return weaponInstance.eval(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &displayStats() {
		static auto ret = statFactory<Formula::FloatNode, DisplayFrm>(Weapon::instanceStats());
		return ret;
	}
}// namespace Modifiers::Weapon