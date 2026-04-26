#include "instance.hpp"

#include "store.hpp"

void Disc::Instance::unequip() const {
	for (auto &[_, agent]: Store::agents) {
		auto &equippedKey = agent.state.equippedLoadout.disc.getSlotted().fromPartition(this->partition);
		if (equippedKey == key) {
			equippedKey.clear();
			agent.state.equippedLoadout.disc.refreshStats();
			agent.updateEvent.notify();
		}
		for (auto &loadout: agent.state.loadouts) {
			std::visit(
				Utils::overloaded{
					[&](Stats::Disc::Slotted &slotted) {
						auto &equippedKey = slotted.fromPartition(this->partition);
						if (equippedKey == key) {
							equippedKey.clear();
							loadout.disc.refreshStats();
							agent.updateEvent.notify();
						}
					},
					[](Stats::Disc::Theorycraft &theorycraft) {},
				},
				loadout.disc.equipped
			);
		}
	}
	this->updateEvent.notify();
}

void Disc::Instance::equipOn(Agent::InstanceKey agentKey, std::optional<uint32_t> loadoutIndex) const {
	if (!agentKey) return;

	auto &agent = Store::agents.at(agentKey);
	auto &loadout = agent.state.loadoutByIndex(loadoutIndex);

	if (!std::holds_alternative<Stats::Disc::Slotted>(loadout.disc.equipped)) return;
	auto &slotted = loadout.disc.getSlotted();

	auto &equippedKey = slotted.fromPartition(this->partition);

	if (!loadoutIndex && equippedKey != this->key) {
		auto alreadyEquippedLocation = equippedOn();
		if (equippedKey && alreadyEquippedLocation) {
			auto &otherAgent = Store::agents.at(alreadyEquippedLocation);
			otherAgent.state.equippedLoadout.disc.getSlotted().fromPartition(this->partition) = equippedKey;
			Store::discs.at(equippedKey).updateEvent.notify();
			otherAgent.updateEvent.notify();
		} else if (alreadyEquippedLocation) {
			auto &otherAgent = Store::agents.at(alreadyEquippedLocation);
			otherAgent.state.equippedLoadout.disc.getSlotted().fromPartition(this->partition).clear();
			otherAgent.updateEvent.notify();
		}
	}

	auto equippedKeyCopy = equippedKey;

	equippedKey = this->key;

	if (equippedKeyCopy) Store::discs.at(equippedKeyCopy).updateEvent.notify();

	loadout.disc.refreshStats();
	agent.updateEvent.notify();
	updateEvent.notify();
}

Agent::InstanceKey Disc::Instance::equippedOn() const {
	Agent::InstanceKey ret{};
	for (auto &[agentKey, agent]: Store::agents) {
		auto &equippedKey = agent.state.equippedLoadout.disc.getSlotted().fromPartition(this->partition);
		if (equippedKey == key) {
			ret = agentKey;
			break;
		}
	}
	return ret;
}

std::vector<Agent::InstanceKey> Disc::Instance::usedOn() const {
	std::vector<Agent::InstanceKey> ret{};
	for (auto &[agentKey, agent]: Store::agents) {
		auto &equippedKey = agent.state.equippedLoadout.disc.getSlotted().fromPartition(this->partition);
		if (equippedKey == key) {
			ret.emplace_back(agentKey);
			continue;
		}
		for (const auto &loadout: agent.state.loadouts) {
			if (!std::holds_alternative<Stats::Disc::Slotted>(loadout.disc.equipped)) continue;
			const auto &discKey = loadout.disc.getSlotted().fromPartition(this->partition);
			if (discKey == this->key) {
				ret.emplace_back(agentKey);
				continue;
			}
		}
	}
	return ret;
}

void Disc::Instance::refreshUsages() const {
	for (auto &[agentKey, agent]: Store::agents) {
		auto &equippedKey = agent.state.equippedLoadout.disc.getSlotted().fromPartition(this->partition);
		if (equippedKey == key) {
			agent.state.equippedLoadout.disc.refreshStats();
			agent.updateEvent.notify();
		}
		for (auto &loadout: agent.state.loadouts) {
			if (!std::holds_alternative<Stats::Disc::Slotted>(loadout.disc.equipped)) continue;
			const auto &discKey = loadout.disc.getSlotted().fromPartition(this->partition);
			if (discKey == this->key) {
				loadout.disc.refreshStats();
				agent.updateEvent.notify();
			}
		}
	}
}
