#include "engine.hpp"
#include "engine/data.hpp"

Serialization::Save::Engine Serialization::Save::Engine::fromInstance(const ::Engine::Instance &engine) {
	return {
		.instanceKey = engine.instanceKey,
		.dataKey = engine.stats.data->key,
		.level = engine.stats.sheet.level,
		.modification = engine.stats.sheet.modification,
		.overclock = engine.stats.sheet.overclock,
	};
}

::Engine::Instance Serialization::Save::Engine::toInstance() const {
	auto instance = ::Engine::Instance(dataKey, instanceKey);
	instance.stats.sheet.level = level;
	instance.stats.sheet.modification = modification;
	instance.stats.sheet.overclock = overclock;

	return instance;
}
