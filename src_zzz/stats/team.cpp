#include "team.hpp"

#include "agent/instance.hpp"
#include "formula/option.hpp"
#include "formula/requires.hpp"


Stats::Team::Team() {
	using namespace Formula::Operators;

	auto IsActive = [](const Utils::HashedString &str) {
		return Formula::impl_IsActivePassive{.name = str};
	};

	// Frostbite
	auto frostbiteCond = IsActive("frostbiteCond");
	auto frostbiteBuff = Formula::Requires{.requirement = frostbiteCond, .ret = Formula::Constant{.value = 0.1f}};
	resonances.cd.modifiers.at(0) = frostbiteBuff;

	options.insert({
		Utils::HashedString("frostbiteCond"),
		Option::Boolean{
			.key = "frostbiteCond",
			.name = "Enemy is affected by Frostbite",
			.mods{.combat{.cd = frostbiteBuff}},
		},
	});
}
[[nodiscard]] bool Stats::Team::hasAgent(::Agent::InstanceKey key) const {
	for (const auto &agent: agents) {
		if (!agent) continue;
		if (agent->instanceKey == key) return true;
	}
	return false;
}
