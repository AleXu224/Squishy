#include "team.hpp"

#include "character/instance.hpp"
#include "ranges"
#include "store.hpp"

Serialization::Save::Team Serialization::Save::Team::fromInstance(const ::Team::Instance &team) {
	return {
		.instanceKey = team.instanceKey,
		.name = team.name,
		.characters = [&]() {
			std::array<std::optional<::Character::InstanceKey>, 4> ret{};
			for (auto [retCharacter, storeCharacter]: std::views::zip(ret, team.stats.characters)) {
				if (!storeCharacter) continue;
				retCharacter = storeCharacter->instanceKey;
			}
			return ret;
		}(),
	};
}

::Team::Instance Serialization::Save::Team::toInstance() const {
	auto instance = ::Team::Instance{
		.instanceKey = instanceKey,
		.name = name,
	};
	instance.stats.characters = [&]() {
		std::array<::Character::Instance *, 4> ret{};
		for (auto [retCharRef, saveChar]: std::views::zip(ret, characters)) {
			if (!saveChar.has_value()) continue;
			retCharRef = &::Store::characters.at(saveChar.value());
		}
		return ret;
	}();

	return instance;
}
