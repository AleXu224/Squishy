#pragma once

#include "character/characters.hpp"
#include "character/instance.hpp"
#include "stats/team.hpp"
#include "store.hpp"
#include "testCharacter.hpp"
#include "testWeapon.hpp"
#include "weapon/weapons.hpp"

[[nodiscard]] inline auto getContext() {
	[[maybe_unused]] static auto _ = []() {
		Character::list = {{::testCharacter.key, ::testCharacter}};
		Weapon::list = {{::testWeapon.key, ::testWeapon}};

		Character::InstanceKey characterKey{1};
		Weapon::InstanceKey weaponKey{1};

		auto weapIt = Store::weapons.insert({weaponKey, Weapon::Instance(::testWeapon.key, weaponKey)});
		auto charIt = Store::characters.insert({characterKey, Character::Instance(characterKey, ::testCharacter.key)});
		charIt.first->second.state.equippedLoadout.swapWeapon(weapIt.first->first);
		Store::teams.insert({{1}, Team::Instance{}});
		Store::enemies.insert({1, Enemy::Instance{}});

		return true;
	}();

	return Formula::Context{
		.source = Store::characters.at({1}).state,
		.active = Store::characters.at({1}).state,
		.team = Store::teams.at({1}).stats,
		.enemy = Store::enemies.at(1).stats,
	};
}

[[nodiscard]] inline auto eval(auto &&evalThing) {
	auto ctx = getContext();

	return evalThing.eval(ctx);
}