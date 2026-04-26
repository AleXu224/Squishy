#include "loadout.hpp"

#include "disc/sets.hpp"
#include "agent/data.hpp"
#include "store.hpp"
#include "engine/data.hpp"


void Stats::State::init() {
	options.clear();
	const auto &agentData = stats.data;
	agentData.getOpts(options);

	equippedLoadout.init(options);

	for (auto &loadout: loadouts) {
		loadout.init(options);
	}

	for (const auto &[setKey, set]: ::Disc::sets) {
		set.getOptions(options);
	}

	stats.sheet.init(*this);
}

void Stats::Loadout::init(Option::TypesMap &options) {
	if (engine) {
		const auto &engineData = engine->data;
		engineData->getOpts(options);
	}

	disc.refreshStats();
}

void Stats::Loadout::swapEngine(::Engine::InstanceKey engineInstanceKey) {
	if (!engineInstanceKey) return;

	auto &engine = ::Store::engines.at(engineInstanceKey);
	this->engineInstanceKey = engineInstanceKey;
	this->engine = &engine.stats;
}
