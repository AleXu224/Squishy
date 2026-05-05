#include "disc.hpp"

#include "ranges"

Serialization::Save::Disc Serialization::Save::Disc::fromInstance(const ::Disc::Instance &disc) {
	std::array<std::optional<DiscSubStat>, 4> subStats{};
	for (auto [subStatData, discSubStat]: std::views::zip(subStats, disc.subStats)) {
		if (!discSubStat.stat) continue;
		subStatData = DiscSubStat{
			.stat = discSubStat.stat.value(),
			.activated = discSubStat.activated,
			.value = discSubStat.value,
		};
	}

	return {
		.instanceKey = disc.key,
		.setKey = disc.set,
		.partition = disc.partition,
		.mainStat = disc.mainStat,
		.subStats = subStats,
		.level = disc.level,
		.rarity = disc.rarity,
	};
}

::Disc::Instance Serialization::Save::Disc::toInstance() const {
	std::array<StatValue, 4> subStats{};
	for (auto [subStatData, discSubStat]: std::views::zip(this->subStats, subStats)) {
		if (!subStatData) continue;
		discSubStat = StatValue{
			.stat = subStatData->stat,
			.activated = subStatData->activated,
			.value = subStatData->value,
		};
	}

	auto instance = ::Disc::Instance{
		.key = instanceKey,
		.set = setKey,
		.partition = partition,
		.mainStat = mainStat,
		.subStats = subStats,
		.level = level,
		.rarity = rarity,
	};
	instance.updateStats();

	return instance;
}
