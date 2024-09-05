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

		Store::weapons.insert({::testWeapon.key, Weapon::Instance(::testWeapon.key)});
		Store::characters.insert({::testCharacter.key, Character::Instance(::testCharacter.key, ::testWeapon.key)});
		Store::teams.insert({0, Team::Instance{}});
		Store::enemies.insert({0, Enemy::Instance{}});

		return true;
	}();

	return Formula::Context{
		.source = Store::characters.at(::testCharacter.key).loadout,
		.target = Store::characters.at(::testCharacter.key).loadout,
		.team = Store::teams.at(0).stats,
		.enemy = Store::enemies.at(0).stats,
	};
}

[[nodiscard]] inline auto eval(auto &&evalThing) {
	auto ctx = getContext();

	return evalThing.eval(ctx);
}