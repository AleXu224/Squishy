#include "instance.hpp"

#include "character/characters.hpp"

Character::Instance::Instance(const InstanceKey &instanceKey, const DataKey &dataKey)
	: instanceKey(instanceKey),
	  dataKey(dataKey),
	  state(Stats::State{
		  .instanceKey = instanceKey,
		  .stats = Stats::Character(Character::list.at(dataKey)),
		  .options{},
		  .equippedLoadout{},
	  }) {
	state.init();
}
