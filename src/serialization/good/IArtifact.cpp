#include "IArtifact.hpp"

#include "SlotKey.hpp"
#include "StatKey.hpp"
#include "artifact/sets.hpp"
#include "character/data.hpp"
#include "ranges"
#include "store.hpp"


Serialization::Good::IArtifact Serialization::Good::IArtifact::fromInstance(const Artifact::Instance &artifact) {
	auto equippedCharacter = artifact.equippedOn();
	return {
		.setKey{::Artifact::sets.at(artifact.set).goodKey},
		.slotKey = Serialization::Good::keySlot.at(artifact.slot),
		.level = artifact.level,
		.rarity = artifact.rarity,
		.mainStatKey = keyStat.at(artifact.mainStat),
		.location{
			equippedCharacter
				? ::Store::characters.at(equippedCharacter).state.stats.data.goodKey
				: ""
		},
		.substats = [&]() {
			auto ret = std::vector<ISubstat>(4);

			for (auto [subStat, dataSubStat]: std::views::zip(artifact.subStats, ret)) {
				if (!subStat.stat.has_value() || !subStat.activated) continue;
				dataSubStat.key = keyStat.at(subStat.stat.value());
				dataSubStat.value = subStat.value * (Utils::isPercentage(subStat.stat) ? 100.f : 1.f);
			}

			return ret;
		}(),
		.unactivatedSubstats = [&]() {
			auto ret = std::vector<ISubstat>(4);

			for (auto [subStat, dataSubStat]: std::views::zip(artifact.subStats, ret)) {
				if (!subStat.stat.has_value() || subStat.activated) continue;
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
		auto it = artifact.subStats.begin();
		for (auto dataIt = substats.begin(); it != artifact.subStats.end() && dataIt != substats.end(); ++dataIt) {
			if (dataIt->key.empty()) {
				continue;
			}
			if (!statKey.contains(dataIt->key)) {
				std::println("Warning: StatKey {} not found", dataIt->key);
				validSubstats = false;
				break;
			}
			if (!it->stat.has_value()) {
				validSubstats = false;
				break;
			}
			const auto &val = *it;
			if (val.stat != statKey.at(dataIt->key) || val.value > dataIt->value) {
				validSubstats = false;
				break;
			}
			++it;
		}
		for (auto dataIt = unactivatedSubstats.begin(); it != artifact.subStats.end() && dataIt != unactivatedSubstats.end(); ++dataIt) {
			if (dataIt->key.empty()) {
				continue;
			}
			if (!statKey.contains(dataIt->key)) {
				std::println("Warning: StatKey {} not found", dataIt->key);
				validSubstats = false;
				break;
			}
			if (!it->stat.has_value()) {
				validSubstats = false;
				break;
			}
			const auto &val = *it;
			if (val.stat != statKey.at(dataIt->key) || val.value > dataIt->value) {
				validSubstats = false;
				break;
			}
			++it;
		}
		for (; it != artifact.subStats.end(); ++it) {
			if (it->stat.has_value()) {
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

	auto it = artifact.subStats.begin();

	for (auto dataIt = substats.begin(); it != artifact.subStats.end() && dataIt != substats.end(); ++dataIt) {
		if (dataIt->key.empty()) continue;
		if (!statKey.contains(dataIt->key)) {
			std::println("Warning: StatKey {} not found", dataIt->key);
			continue;
		}

		auto stat = statKey.at(dataIt->key);

		*it = StatValue{
			.stat = stat,
			.activated = true,
			.value = dataIt->value / (Utils::isPercentage(stat) ? 100.f : 1.f),
		};
		it++;
	}
	for (auto dataIt = unactivatedSubstats.begin(); it != artifact.subStats.end() && dataIt != unactivatedSubstats.end(); ++dataIt) {
		if (dataIt->key.empty()) continue;
		if (!statKey.contains(dataIt->key)) {
			std::println("Warning: StatKey {} not found", dataIt->key);
			continue;
		}

		auto stat = statKey.at(dataIt->key);

		*it = StatValue{
			.stat = stat,
			.activated = false,
			.value = dataIt->value / (Utils::isPercentage(stat) ? 100.f : 1.f),
		};
		it++;
	}

	artifact.updateStats();

	Character::InstanceKey equippedCharacter{};
	for (auto &[_, character]: ::Store::characters) {
		if (character.state.stats.data.goodKey == location) {
			equippedCharacter = character.instanceKey;
			break;
		}
	}
	if (equippedCharacter) {
		artifact.equipOn(equippedCharacter);
	}
}
