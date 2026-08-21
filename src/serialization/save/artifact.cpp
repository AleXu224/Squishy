#include "artifact.hpp"

#include "ranges"

Serialization::Save::Artifact Serialization::Save::Artifact::fromInstance(const ::Artifact::Instance &artifact) {
	std::array<std::optional<ArtifactSubStat>, 4> subStats{};
	for (auto [subStatData, artifactSubStat]: std::views::zip(subStats, artifact.subStats)) {
		if (!artifactSubStat.stat) continue;
		subStatData = ArtifactSubStat{
			.stat = artifactSubStat.stat.value(),
			.activated = artifactSubStat.activated,
			.value = artifactSubStat.value,
			.initialValue = artifactSubStat.initialValue,
		};
	}

	return {
		.instanceKey = artifact.key,
		.setKey = artifact.set,
		.slot = artifact.slot,
		.mainStat = artifact.mainStat,
		.subStats = subStats,
		.level = artifact.level,
		.rarity = artifact.rarity,
		.totalRolls = artifact.totalRolls,
	};
}

::Artifact::Instance Serialization::Save::Artifact::toInstance() const {
	std::array<StatValue, 4> subStats{};
	for (auto [subStatData, artifactSubStat]: std::views::zip(this->subStats, subStats)) {
		if (!subStatData) continue;
		artifactSubStat = StatValue{
			.stat = subStatData->stat,
			.activated = subStatData->activated,
			.value = subStatData->value,
			.initialValue = subStatData->initialValue,
		};
	}

	auto instance = ::Artifact::Instance{
		.key = instanceKey,
		.set = setKey,
		.slot = slot,
		.mainStat = mainStat,
		.subStats = subStats,
		.level = level,
		.rarity = rarity,
		.totalRolls = totalRolls,
	};
	instance.updateStats();

	return instance;
}
