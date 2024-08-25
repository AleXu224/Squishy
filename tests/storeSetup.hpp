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

		return true;
	}();
	Stats::Team testTeam{};

    auto &character = Store::characters.at(::testCharacter.key);
    testTeam.characters.at(0) = character;

    return std::pair{testTeam, character};
}

[[nodiscard]] inline auto eval(auto &&evalThing) {
    const auto&[team, character] = getCharacter();

    return evalThing.eval(character.stats, character.stats, team);
}