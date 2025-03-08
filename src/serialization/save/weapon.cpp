#include "weapon.hpp"
#include "weapon/data.hpp"

Serialization::Save::Weapon Serialization::Save::Weapon::fromInstance(const ::Weapon::Instance &weapon) {
	return {
		.instanceKey = weapon.instanceKey,
		.dataKey = weapon.stats.data->key,
		.level = weapon.stats.sheet.level,
		.ascension = weapon.stats.sheet.ascension,
		.refinement = weapon.stats.sheet.refinement,
		.options = optionsFromInstance(weapon.stats.options),
	};
}

::Weapon::Instance Serialization::Save::Weapon::toInstance() const {
	auto instance = ::Weapon::Instance(dataKey, instanceKey);
	instance.stats.sheet.level = level;
	instance.stats.sheet.ascension = ascension;
	instance.stats.sheet.refinement = refinement;
	optionsToInstance(options, instance.stats.options);

	return instance;
}
