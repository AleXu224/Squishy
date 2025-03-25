#pragma once

#include "artifact/instance.hpp"
#include "character/instance.hpp"
#include "enemy/instance.hpp"
#include "stats/team.hpp"
#include "team/instance.hpp"
#include "unordered_map"
#include "weapon/instance.hpp"

#include "serialization/good/IGOOD.hpp"

namespace Store {
	inline std::unordered_map<Character::InstanceKey, Character::Instance> characters{};
	inline std::unordered_map<Team::InstanceKey, Team::Instance> teams{};
	inline std::unordered_map<Enemy::Key, Enemy::Instance> enemies{};
	inline std::unordered_map<Weapon::InstanceKey, Weapon::Instance> weapons{};
	inline std::unordered_map<Artifact::InstanceKey, Artifact::Instance> artifacts{};

	static inline Team::Instance defaultTeam{
		.instanceKey{std::numeric_limits<uint32_t>::max()},
		.name = "Default team",
	};

	inline uint32_t lastArtifactId = 1;
	inline uint32_t lastWeaponId = 1;
	inline uint32_t lastCharacterId = 1;
	inline uint32_t lastTeamId = 1;
	inline uint32_t lastComboId = 1;

	inline squi::VoidObservable artifactListUpdateEvent{};
	inline squi::VoidObservable weaponListUpdateEvent{};
	inline squi::VoidObservable characterListUpdateEvent{};
	inline squi::VoidObservable teamListUpdateEvent{};

	Character::Instance &createCharacter(Character::DataKey dataKey, Weapon::InstanceKey weapon = {});
	Weapon::Instance &createWeapon(Weapon::DataKey dataKey);
	Artifact::Instance &createArtifact(Artifact::SetKey setKey);
	Team::Instance &createTeam(std::string_view name = "");

	extern void saveToFile(const std::string &path);
	extern void loadFromFile(const std::string &path);

	extern Serialization::Good::IGOOD saveToGOOD();
	extern void loadFromGOOD(const Serialization::Good::IGOOD &);
}// namespace Store
