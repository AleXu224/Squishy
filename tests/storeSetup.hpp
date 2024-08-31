#pragma once

#include "character/characters.hpp"
#include "character/instance.hpp"
#include "stats/team.hpp"
#include "store.hpp"
#include "testCharacter.hpp"
#include "testWeapon.hpp"
#include "weapon/weapons.hpp"


[[nodiscard]] inline auto getCharacter() {
	[[maybe_unused]] static auto _ = []() {
		Character::list = {{::testCharacter.key, ::testCharacter}};
		Weapon::list = {{::testWeapon.key, ::testWeapon}};

		Store::weapons.insert({::testWeapon.key, Weapon::Instance(::testWeapon.key)});
		Store::characters.insert({::testCharacter.key, Character::Instance(::testCharacter.key, ::testWeapon.key)});
		Store::teams.insert({0, Team::Instance{}});
		Store::enemies.insert({0, Enemy::Instance{}});

		return true;
	}();
	auto &team = Store::teams.at(0);
	auto &enemy = Store::enemies.at(0);

	auto &character = Store::characters.at(::testCharacter.key);
	team.stats.characters.at(0) = character;

	return std::tuple{team.stats, enemy.stats, character};
}

[[nodiscard]] inline auto eval(auto &&evalThing) {
	const auto &[team, enemy, character] = getCharacter();

	Formula::Context ctx{
		.source = character.stats,
		.target = character.stats,
		.team = team,
		.enemy = enemy,
	};

	return evalThing.eval(ctx);
}