#include "IDisc.hpp"

#include "RarityKey.hpp"
#include "SlotKey.hpp"
#include "StatKey.hpp"
#include "agent/data.hpp"
#include "disc/sets.hpp"
#include "ranges"
#include "store.hpp"


Serialization::Zod::IDisc Serialization::Zod::IDisc::fromInstance(const Disc::Instance &disc) {
	auto equippedAgent = disc.equippedOn();
	return {
		.setKey{::Disc::sets.at(disc.set).goodKey},
		.slotKey = Serialization::Zod::keySlot.at(disc.partition),
		.level = disc.level,
		.rarity = keyRarity.at(disc.rarity),
		.mainStatKey = keyStat.at(disc.mainStat),
		.location{
			equippedAgent
				? ::Store::agents.at(equippedAgent).state.stats.data.goodKey
				: ""
		},
		.substats = [&]() {
			auto ret = std::vector<ISubstat>(4);

			for (auto [subStat, dataSubStat]: std::views::zip(disc.subStats, ret)) {
				if (!subStat.stat.has_value() || !subStat.activated) continue;
				dataSubStat.key = keyStat.at(subStat.stat.value());
				dataSubStat.upgrades = std::lround(subStat.value / Stats::Values::subStat.at(subStat.stat.value()).at(disc.rarity));
			}

			return ret;
		}(),
	};
}

std::expected<std::reference_wrapper<Disc::Instance>, std::string> Serialization::Zod::IDisc::createInstance() const {
	auto discData = getData();
	if (!discData) return std::unexpected(discData.error());
	auto &data = discData.value().get();

	auto &disc = Store::createDisc(data.key);

	disc.partition = Serialization::Zod::slotKey.at(slotKey);
	disc.rarity = rarityKey.at(rarity);
	disc.mainStat = statKey.at(mainStatKey);

	writeToInstance(disc);

	return disc;
}

std::expected<std::reference_wrapper<const Disc::Set>, std::string> Serialization::Zod::IDisc::getData() const {
	for (const auto &[_, data]: Disc::sets) {
		if (data.goodKey == setKey) return data;
	}
	return std::unexpected(std::format("Data for \"{}\" not found", setKey));
}

std::expected<std::reference_wrapper<Disc::Instance>, std::string> Serialization::Zod::IDisc::isAlreadyStored() const {
	auto setExp = getData();
	if (!setExp) return std::unexpected(setExp.error());
	const auto &set = setExp.value().get();
	for (auto &[_, disc]: Store::discs) {
		if (set.goodKey != setKey) continue;
		if (Serialization::Zod::slotKey.at(slotKey) != disc.partition) continue;
		if (disc.level > level) continue;
		if (disc.rarity != rarityKey.at(rarity)) continue;
		if (statKey.at(mainStatKey) != disc.mainStat) continue;
		bool validSubstats = true;
		auto it = disc.subStats.begin();
		for (auto dataIt = substats.begin(); it != disc.subStats.end() && dataIt != substats.end(); ++dataIt) {
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
			if (val.stat != statKey.at(dataIt->key) || val.value > (dataIt->upgrades * Stats::Values::subStat.at(statKey.at(dataIt->key)).at(disc.rarity))) {
				validSubstats = false;
				break;
			}
			++it;
		}
		for (; it != disc.subStats.end(); ++it) {
			if (it->stat.has_value()) {
				validSubstats = false;
				break;
			}
		}
		if (!validSubstats) continue;

		return disc;
	}

	return std::unexpected("Disc not found");
}

void Serialization::Zod::IDisc::writeToInstance(Disc::Instance &disc) const {
	disc.level = level;

	for (auto &subStat: disc.subStats) {
		subStat.stat = std::nullopt;
	}

	auto it = disc.subStats.begin();

	for (auto dataIt = substats.begin(); it != disc.subStats.end() && dataIt != substats.end(); ++dataIt) {
		if (dataIt->key.empty()) continue;
		if (!statKey.contains(dataIt->key)) {
			std::println("Warning: StatKey {} not found", dataIt->key);
			continue;
		}

		auto stat = statKey.at(dataIt->key);

		*it = StatValue{
			.stat = stat,
			.activated = true,
			.value = dataIt->upgrades * Stats::Values::subStat.at(stat).at(disc.rarity),
		};
		it++;
	}

	disc.updateStats();

	Agent::InstanceKey equippedAgent{};
	for (auto &[_, agent]: ::Store::agents) {
		if (agent.state.stats.data.goodKey == location) {
			equippedAgent = agent.instanceKey;
			break;
		}
	}
	if (equippedAgent) {
		disc.equipOn(equippedAgent);
	}
}
