#include "agent.hpp"
#include "engine/data.hpp"
#include "store.hpp"


Serialization::Save::Agent Serialization::Save::Agent::fromInstance(const ::Agent::Instance &agent) {
	return {
		.instanceKey = agent.instanceKey,
		.dataKey = agent.dataKey,
		.mindscape = agent.state.stats.sheet.mindscape,
		.level = agent.state.stats.sheet.level,
		.promotion = agent.state.stats.sheet.promotion,
		.basicLevel = agent.state.stats.sheet.skills.basic.constant,
		.dodgeLevel = agent.state.stats.sheet.skills.dodge.constant,
		.assistLevel = agent.state.stats.sheet.skills.assist.constant,
		.specialLevel = agent.state.stats.sheet.skills.special.constant,
		.chainLevel = agent.state.stats.sheet.skills.chain.constant,
		.coreLevel = agent.state.stats.sheet.skills.core.constant,
		.loadoutIndex = agent.state.loadoutIndex,
		.equippedLoadout = Serialization::Save::Loadout::fromInstance(agent.state.equippedLoadout),
		.options = optionsFromInstance(agent.state.options),
		.combos = comboFromInstance(agent.combos),
		.loadouts = [&]() {
			std::vector<::Serialization::Save::Loadout> ret;
			ret.reserve(agent.state.loadouts.size());
			for (const auto &loadout: agent.state.loadouts) {
				ret.emplace_back(::Serialization::Save::Loadout::fromInstance(loadout));
			}
			return ret;
		}(),
		.optimizationOptions = Optimization::fromInstance(*agent.optimizationOptions),
	};
}

::Agent::Instance Serialization::Save::Agent::toInstance() const {
	auto instance = ::Agent::Instance(instanceKey, dataKey);

	instance.state.stats.sheet.mindscape = mindscape;
	instance.state.stats.sheet.level = level;
	instance.state.stats.sheet.promotion = promotion;
	instance.state.stats.sheet.skills.basic.constant = basicLevel;
	instance.state.stats.sheet.skills.dodge.constant = dodgeLevel;
	instance.state.stats.sheet.skills.assist.constant = assistLevel;
	instance.state.stats.sheet.skills.special.constant = specialLevel;
	instance.state.stats.sheet.skills.chain.constant = chainLevel;
	instance.state.stats.sheet.skills.core.constant = coreLevel;
	instance.state.loadoutIndex = loadoutIndex;
	instance.state.equippedLoadout = equippedLoadout.toInstance(instance.state.stats.base.specialty);

	for (const auto &partition: ::Disc::partitions) {
		auto &equipped = instance.state.equippedLoadout.disc.getSlotted().fromPartition(partition);
		if (!equipped) continue;
		if (!::Store::discs.contains(equipped)) equipped.clear();
	}

	instance.combos = comboToInstance(combos);
	instance.optimizationOptions = std::make_shared<::Optimization::Options>(optimizationOptions.toInstance());

	instance.state.equippedLoadout.disc.refreshStats();

	std::vector<::Stats::Loadout> loadouts{};
	for (const auto &loadout: this->loadouts) {
		auto &entry = loadouts.emplace_back(loadout.toInstance(instance.state.stats.base.specialty));
		entry.engine->data->getOpts(instance.state.options);

		// Check if the disc exists
		if (!std::holds_alternative<::Stats::Disc::Slotted>(entry.disc.equipped)) continue;
		auto &slotted = std::get<::Stats::Disc::Slotted>(entry.disc.equipped);
		for (const auto &partition: ::Disc::partitions) {
			auto &equipped = slotted.fromPartition(partition);
			if (!equipped) continue;
			if (!::Store::discs.contains(equipped)) equipped.clear();
		}
	}
	instance.state.loadouts = loadouts;

	instance.state.init();
	optionsToInstance(options, instance.state.options);

	return instance;
}
