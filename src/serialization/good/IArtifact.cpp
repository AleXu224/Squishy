#include "IArtifact.hpp"

#include "SlotKey.hpp"
#include "StatKey.hpp"
#include "ranges"
#include "store.hpp"


std::expected<std::reference_wrapper<Artifact::Instance>, std::string> Serialization::Good::IArtifact::createInstance() const {
	auto artifactData = getData();
	if (!artifactData) return std::unexpected(artifactData.error());
	auto &data = artifactData.value().get();

	auto &artifact = Store::createArtifact(data.key);

	artifact.slot = Serialization::Good::slotKey.at(slotKey);
	artifact.rarity = rarity;
	artifact.mainStat = statKey.at(mainStatKey);

	writeToInstance(artifact);

	return artifact;
}

std::expected<std::reference_wrapper<const Artifact::Set>, std::string> Serialization::Good::IArtifact::getData() const {
	for (const auto &[_, data]: Artifact::sets) {
		if (data.goodKey == setKey) return data;
	}
	return std::unexpected(std::format("Data for \"{}\" not found", setKey));
}

std::expected<std::reference_wrapper<Artifact::Instance>, std::string> Serialization::Good::IArtifact::isAlreadyStored() const {
	auto setExp = getData();
	if (!setExp) return std::unexpected(setExp.error());
	const auto &set = setExp.value().get();
	for (auto [_, artifact]: Store::artifacts) {
		if (set.goodKey != setKey) continue;
		if (Serialization::Good::slotKey.at(slotKey) != artifact.slot) continue;
		if (artifact.level > level) continue;
		if (artifact.rarity != rarity) continue;
		if (statKey.at(mainStatKey) != artifact.mainStat) continue;
		bool validSubstats = true;
		for (auto [subStat, dataSubStat]: std::views::zip(artifact.subStats, substats)) {
			if (statKey.empty()) {
				if (subStat.has_value()) {
					validSubstats = false;
					break;
				}
				continue;
			}
			if (!statKey.contains(dataSubStat.key)) {
				std::println("Warning: StatKey {} not found", dataSubStat.key);
				validSubstats = false;
				break;
			}
			if (!subStat.has_value()) {
				validSubstats = false;
				break;
			}
			const auto &val = subStat.value();
			if (val.stat != statKey.at(dataSubStat.key) || val.value > dataSubStat.value) {
				validSubstats = false;
				break;
			}
		}
		if (!validSubstats) continue;

		return artifact;
	}

	return std::unexpected("Artifact not found");
}

void Serialization::Good::IArtifact::writeToInstance(Artifact::Instance &artifact) const {
	artifact.level = level;

	for (auto &subStat: artifact.subStats) {
		subStat = std::nullopt;
	}

	for (auto [subStat, dataSubStat]: std::views::zip(artifact.subStats, substats)) {
		if (statKey.empty()) continue;
		if (!statKey.contains(dataSubStat.key)) {
			std::println("Warning: StatKey {} not found", dataSubStat.key);
			continue;
		}

		subStat = StatValue{
			.stat = statKey.at(dataSubStat.key),
			.value = dataSubStat.value,
		};
	}
}
