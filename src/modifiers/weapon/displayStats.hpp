#pragma once

#include "instance.hpp"
#include "passive.hpp"

namespace Modifiers::Weapon {
	template<auto weaponPassivePre, auto weaponPassivePost, auto weaponInstance>
	struct Frm {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step prevStep) const {
			return (weaponPassivePre + weaponPassivePost + weaponInstance).print(context, prevStep);
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return weaponPassivePre.eval(context)
				 + weaponPassivePost.eval(context)
				 + weaponInstance.eval(context);
		}
	};

	static constexpr StatFactory<Frm, Weapon::Passive::preMods, Weapon::Passive::postMods, Weapon::instanceStats> displayStats;
}// namespace Modifiers::Weapon