#include "artifact/instance.hpp"
#include "artifact/sets.hpp"// IWYU pragma: keep
#include "artifact/sets/GildedDreams.hpp"
#include "character/characters.hpp"
#include "character/characters/Cyno.hpp"
#include "src/formula/node.hpp"
#include "src/stats/helpers.hpp"
#include "store.hpp"
#include "weapon/weapons/StaffOfTheScarletSands.hpp"

#include "benchmark/benchmark.h"


static void formulaCalc(benchmark::State &state) {
	static_assert(Stats::SheetLike<Stats::CharacterSheet>, "Character sheet must be SheetLike");
	static_assert(Stats::SheetLike<Stats::WeaponSheet>, "Character sheet must be SheetLike");

	Weapon::initWeapons();
	Character::initCharacters();
	Artifact::initArtifacts();

	auto &weapon = Store::weapons.insert({Weapon::Datas::staffOfTheScarletSands.key, Weapon::Instance(Weapon::Datas::staffOfTheScarletSands.key)}).first->second;
	weapon.stats.sheet.level = 90;
	weapon.stats.sheet.ascension = 6;
	weapon.stats.sheet.refinement = 0;
	auto &character = Store::characters.insert({Character::Datas::cyno.key, Character::Instance(Character::Datas::cyno.key, Weapon::Datas::staffOfTheScarletSands.key)}).first->second;
	character.stats.character.sheet.level = 90;
	character.stats.character.sheet.ascension = 6;
	character.stats.character.sheet.talents.burst = 9;
	character.stats.character.sheet.talents.skill = 9;
	character.stats.character.sheet.constellation = 1;
	character.stats.artifact.equipped = {1, 2, 3, 4, 5};

	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
			.set = Artifact::Sets::gildedDreams.key,
			.slot = Artifact::Slot::flower,
			.mainStat = Stat::hp,
			.subStats = {
				StatValue{Stat::cr, 0.062f},
				StatValue{Stat::def_, 0.058f},
				StatValue{Stat::cd, 0.202f},
				StatValue{Stat::er, 0.123f},
			},
			.level = 20,
			.rarity = Rarity::fiveStar,
			.equippedCharacter = Character::Datas::cyno.key,
		},
	});
	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
			.set = Artifact::Sets::gildedDreams.key,
			.slot = Artifact::Slot::plume,
			.mainStat = Stat::atk,
			.subStats = {
				StatValue{Stat::def, 19.f},
				StatValue{Stat::hp, 269.f},
				StatValue{Stat::cd, 0.148f},
				StatValue{Stat::cr, 0.132f},
			},
			.level = 20,
			.rarity = Rarity::fiveStar,
			.equippedCharacter = Character::Datas::cyno.key,
		},
	});
	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
			.set = Artifact::Sets::gildedDreams.key,
			.slot = Artifact::Slot::sands,
			.mainStat = Stat::em,
			.subStats = {
				StatValue{Stat::cr, 0.101f},
				StatValue{Stat::er, 0.104f},
				StatValue{Stat::cd, 0.070f},
				StatValue{Stat::def_, 0.139f},
			},
			.level = 20,
			.rarity = Rarity::fiveStar,
			.equippedCharacter = Character::Datas::cyno.key,
		},
	});
	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
			.set = Artifact::Sets::gildedDreams.key,
			.slot = Artifact::Slot::goblet,
			.mainStat = Stat::electroDmg,
			.subStats = {
				StatValue{Stat::atk_, 0.152f},
				StatValue{Stat::def, 37.f},
				StatValue{Stat::cr, 0.078f},
				StatValue{Stat::cd, 0.124f},
			},
			.level = 20,
			.rarity = Rarity::fiveStar,
			.equippedCharacter = Character::Datas::cyno.key,
		},
	});
	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
			.set = Artifact::Sets::gildedDreams.key,
			.slot = Artifact::Slot::circlet,
			.mainStat = Stat::cd,
			.subStats = {
				StatValue{Stat::atk_, 0.058f},
				StatValue{Stat::cr, 0.109f},
				StatValue{Stat::hp, 598.0f},
				StatValue{Stat::em, 47.f},
			},
			.level = 20,
			.rarity = Rarity::fiveStar,
			.equippedCharacter = Character::Datas::cyno.key,
		},
	});

	character.getArtifactStats();

    auto &node = character.stats.character.data.nodes.burst.at(0);

    for (auto _ : state) {
        (void) node.formula.eval(character.stats);
    }
}
BENCHMARK(formulaCalc);

BENCHMARK_MAIN();