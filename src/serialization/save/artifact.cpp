#include "artifact.hpp"

#include "ranges"

Serialization::Save::Artifact Serialization::Save::Artifact::fromInstance(const ::Artifact::Instance &artifact) {
	std::array<std::optional<ArtifactSubStat>, 4> subStats{};
	for (auto [subStatData, artifactSubStat]: std::views::zip(subStats, artifact.subStats)) {
		if (!artifactSubStat) continue;
		subStatData = ArtifactSubStat{
			.stat = artifactSubStat->stat,
			.value = artifactSubStat->value,
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
		.equippedCharacter = artifact.equippedCharacter,
	};
}

::Artifact::Instance Serialization::Save::Artifact::toInstance() const {
	std::array<std::optional<StatValue>, 4> subStats{};
	for (auto [subStatData, artifactSubStat]: std::views::zip(this->subStats, subStats)) {
		if (!subStatData) continue;
		artifactSubStat = StatValue{
			.stat = subStatData->stat,
			.value = subStatData->value,
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
		.equippedCharacter = equippedCharacter,
	};
	instance.updateStats();

	return instance;
}
