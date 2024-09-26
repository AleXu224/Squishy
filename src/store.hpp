#pragma once

#include "artifact/instance.hpp"
#include "character/instance.hpp"
#include "enemy/instance.hpp"
#include "stats/team.hpp"
#include "team/instance.hpp"
#include "unordered_map"
#include "weapon/instance.hpp"

#include "serialization/save/save.hpp"

#include "filesystem"


namespace Store {
	inline std::unordered_map<Character::InstanceKey, Character::Instance> characters{};
	inline std::unordered_map<Team::InstanceKey, Team::Instance> teams{};
	inline std::unordered_map<Enemy::Key, Enemy::Instance> enemies{};
	inline std::unordered_map<Weapon::InstanceKey, Weapon::Instance> weapons{};
	inline std::unordered_map<Artifact::InstanceKey, Artifact::Instance> artifacts{};

	inline uint32_t lastArtifactId = 1;
	inline uint32_t lastWeaponId = 1;
	inline uint32_t lastCharacterId = 1;
	inline uint32_t lastTeamId = 1;

	inline squi::VoidObservable artifactListUpdateEvent{};
	inline squi::VoidObservable weaponListUpdateEvent{};
	inline squi::VoidObservable characterListUpdateEvent{};
	inline squi::VoidObservable teamListUpdateEvent{};

	[[nodiscard]] extern auto serializeOptions(auto &&options);

	extern void deserializeOptions(const std::vector<Serialization::Save::OptionTypes> &options, auto &&targetMap);

	[[nodiscard]] extern Serialization::Save::Save save();

	extern void load(const Serialization::Save::Save &save);

	extern void saveToFile(const std::filesystem::path &path);

	extern void loadFromFile(const std::filesystem::path &path);
}// namespace Store
