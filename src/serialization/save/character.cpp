#include "character.hpp"
#include "store.hpp"
#include "weapon/data.hpp"

Serialization::Save::Character Serialization::Save::Character::fromInstance(const ::Character::Instance &character) {
	return {
		.instanceKey = character.instanceKey,
		.dataKey = character.dataKey,
		.constellation = character.state.stats.sheet.constellation,
		.level = character.state.stats.sheet.level,
		.ascension = character.state.stats.sheet.ascension,
		.normalLevel = character.state.stats.sheet.talents.normal.constant,
		.skillLevel = character.state.stats.sheet.talents.skill.constant,
		.burstLevel = character.state.stats.sheet.talents.burst.constant,
		.loadoutIndex = character.state.loadoutIndex,
		.equippedLoadout = Serialization::Save::Loadout::fromInstance(character.state.equippedLoadout),
		.options = optionsFromInstance(character.state.options),
		.combos = comboFromInstance(character.combos),
		.loadouts = [&]() {
			std::vector<::Serialization::Save::Loadout> ret;
			ret.reserve(character.state.loadouts.size());
			for (const auto &loadout: character.state.loadouts) {
				ret.emplace_back(::Serialization::Save::Loadout::fromInstance(loadout));
			}
			return ret;
		}(),
		.optimizationOptions = Optimization::fromInstance(*character.optimizationOptions),
	};
}

::Character::Instance Serialization::Save::Character::toInstance() const {
	auto instance = ::Character::Instance(instanceKey, dataKey);

	instance.state.stats.sheet.constellation = constellation;
	instance.state.stats.sheet.level = level;
	instance.state.stats.sheet.ascension = ascension;
	instance.state.stats.sheet.talents.normal.constant = normalLevel;
	instance.state.stats.sheet.talents.skill.constant = skillLevel;
	instance.state.stats.sheet.talents.burst.constant = burstLevel;
	instance.state.loadoutIndex = loadoutIndex;
	instance.state.equippedLoadout = equippedLoadout.toInstance(instance.state.stats.base.weaponType);

	for (const auto &slot: ::Artifact::slots) {
		auto &equipped = instance.state.equippedLoadout.artifact.getSlotted().fromSlot(slot);
		if (!equipped) continue;
		if (!::Store::artifacts.contains(equipped)) equipped.clear();
	}

	instance.combos = comboToInstance(combos);
	instance.optimizationOptions = std::make_shared<::Optimization::Options>(optimizationOptions.toInstance());

	instance.state.equippedLoadout.artifact.refreshStats();

	std::vector<::Stats::Loadout> loadouts{};
	for (const auto &loadout: this->loadouts) {
		auto &entry = loadouts.emplace_back(loadout.toInstance(instance.state.stats.base.weaponType));
		entry.weapon->data->getOpts(instance.state.options);

		// Check if the artifact exists
		if (!std::holds_alternative<::Stats::Artifact::Slotted>(entry.artifact.equipped)) continue;
		auto &slotted = std::get<::Stats::Artifact::Slotted>(entry.artifact.equipped);
		for (const auto &slot: ::Artifact::slots) {
			auto &equipped = slotted.fromSlot(slot);
			if (!equipped) continue;
			if (!::Store::artifacts.contains(equipped)) equipped.clear();
		}
	}
	instance.state.loadouts = loadouts;

	instance.state.init();
	optionsToInstance(options, instance.state.options);

	return instance;
}
