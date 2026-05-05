#include "team.hpp"

#include "agent/instance.hpp"
#include "ranges"
#include "store.hpp"

Serialization::Save::Team Serialization::Save::Team::fromInstance(const ::Team::Instance &team) {
	return {
		.instanceKey = team.instanceKey,
		.name = team.name,
		.agents = [&]() {
			std::array<std::optional<::Agent::InstanceKey>, 3> ret{};
			for (auto [retAgent, storeAgent]: std::views::zip(ret, team.stats.agents)) {
				if (!storeAgent) continue;
				retAgent = storeAgent->instanceKey;
			}
			return ret;
		}(),
		.activeAgentIndex = team.stats.activeAgentIndex,
		.options = optionsFromInstance(team.stats.options),
	};
}

::Team::Instance Serialization::Save::Team::toInstance() const {
	auto instance = ::Team::Instance{
		.instanceKey = instanceKey,
		.name = name,
	};
	optionsToInstance(options, instance.stats.options);
	instance.stats.agents = [&]() {
		std::array<::Agent::Instance *, 3> ret{};
		for (auto [retCharRef, saveChar]: std::views::zip(ret, agents)) {
			if (!saveChar.has_value()) continue;
			retCharRef = &::Store::agents.at(saveChar.value());
		}
		return ret;
	}();
	instance.stats.activeAgentIndex = activeAgentIndex;

	return instance;
}
