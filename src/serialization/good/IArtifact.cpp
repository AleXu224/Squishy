#include "IArtifact.hpp"

#include "SlotKey.hpp"
#include "StatKey.hpp"
#include "artifact/sets.hpp"
#include "character/data.hpp"
#include "ranges"
#include "store.hpp"


Serialization::Good::IArtifact Serialization::Good::IArtifact::fromInstance(const Artifact::Instance &artifact) {
	return {
		.setKey{::Artifact::sets.at(artifact.set).goodKey},
		.slotKey = Serialization::Good::keySlot.at(artifact.slot),
		.level = artifact.level,
		.rarity = artifact.rarity,
		.mainStatKey = keyStat.at(artifact.mainStat),
		.location{
			artifact.equippedCharacter
				? ::Store::characters.at(artifact.equippedCharacter).loadout.character.data.goodKey
				: ""
		},
		.substats = [&]() {
			auto ret = std::vector<ISubstat>(4);

			for (auto [subStat, dataSubStat]: std::views::zip(artifact.subStats, ret)) {
				if (!subStat.stat.has_value()) continue;
				dataSubStat.key = keyStat.at(subStat.stat.value());
				dataSubStat.value = subStat.value * (Utils::isPercentage(subStat.stat) ? 100.f : 1.f);
			}

			return ret;
		}(),
	};
}

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
	for (auto &[_, artifact]: Store::artifacts) {
		if (set.goodKey != setKey) continue;
		if (Serialization::Good::slotKey.at(slotKey) != artifact.slot) continue;
		if (artifact.level > level) continue;
		if (artifact.rarity != rarity) continue;
		if (statKey.at(mainStatKey) != artifact.mainStat) continue;
		bool validSubstats = true;
		for (auto [subStat, dataSubStat]: std::views::zip(artifact.subStats, substats)) {
			if (dataSubStat.key.empty()) {
				if (subStat.stat.has_value()) {
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
			if (!subStat.stat.has_value()) {
				validSubstats = false;
				break;
			}
			const auto &val = subStat;
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
		subStat.stat = std::nullopt;
	}

	for (auto [subStat, dataSubStat]: std::views::zip(artifact.subStats, substats)) {
		if (dataSubStat.key.empty()) continue;
		if (!statKey.contains(dataSubStat.key)) {
			std::println("Warning: StatKey {} not found", dataSubStat.key);
			continue;
		}

		auto stat = statKey.at(dataSubStat.key);

		subStat = StatValue{
			.stat = stat,
			.value = dataSubStat.value / (Utils::isPercentage(stat) ? 100.f : 1.f),
		};
	}

	artifact.updateStats();

	Character::InstanceKey equippedCharacter{};
	for (auto &[_, character]: ::Store::characters) {
		if (character.loadout.character.data.goodKey == location) {
			equippedCharacter = character.instanceKey;
			break;
		}
	}
	if (equippedCharacter) {
		artifact.equipOn(equippedCharacter);
	}
}
