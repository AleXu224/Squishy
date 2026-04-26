#include "instance.hpp"

#include "agent/agents.hpp"

Agent::Instance::Instance(const InstanceKey &instanceKey, const DataKey &dataKey)
	: instanceKey(instanceKey),
	  dataKey(dataKey),
	  state(Stats::State{
		  .instanceKey = instanceKey,
		  .stats = Stats::Agent(Agent::list.at(dataKey)),
		  .options{},
		  .equippedLoadout{},
	  }) {
	state.init();
}
