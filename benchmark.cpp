#include "artifact/instance.hpp"
#include "artifact/sets.hpp"// IWYU pragma: keep
#include "character/characters.hpp"
#include "formula/constant.hpp"
#include "src/formula/node.hpp"
#include "stats/team.hpp"
#include "store.hpp"
#include <random>


#include "benchmark/benchmark.h"

[[nodiscard]] Character::Instance &getCharacter() {
	auto weaponData = Weapon::list.begin()->second;
	auto characterData = Character::list.begin()->second;
	auto artifactData = Artifact::sets.begin()->second;
	Weapon::InstanceKey weaponKey{0};
	Character::InstanceKey characterKey{0};
	auto &weapon = Store::weapons.insert({weaponKey, Weapon::Instance(weaponData.key, weaponKey)}).first->second;
	weapon.stats.sheet.level = 90;
	weapon.stats.sheet.ascension = 6;
	weapon.stats.sheet.refinement = 1;
	auto &character = Store::characters.insert({characterKey, Character::Instance(characterKey, characterData.key, weaponKey)}).first->second;
	character.loadout.character.sheet.level = 90;
	character.loadout.character.sheet.ascension = 6;
	character.loadout.character.sheet.talents.burst.constant = 9;
	character.loadout.character.sheet.talents.skill.constant = 9;
	character.loadout.character.sheet.constellation = 0;
	Artifact::InstanceKey artiKey1{1};
	Artifact::InstanceKey artiKey2{2};
	Artifact::InstanceKey artiKey3{3};
	Artifact::InstanceKey artiKey4{4};
	Artifact::InstanceKey artiKey5{5};
	character.loadout.artifact.equipped = {artiKey1, artiKey2, artiKey3, artiKey4, artiKey5};

	Store::artifacts.insert({
		{++Store::lastArtifactId},
		Artifact::Instance{
			.key = {Store::lastArtifactId},
			.set = artifactData.key,
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
			.equippedCharacter = characterKey,
		},
	});
	Store::artifacts.insert({
		{++Store::lastArtifactId},
		Artifact::Instance{
			.key = {Store::lastArtifactId},
			.set = artifactData.key,
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
			.equippedCharacter = characterKey,
		},
	});
	Store::artifacts.insert({
		{++Store::lastArtifactId},
		Artifact::Instance{
			.key = {Store::lastArtifactId},
			.set = artifactData.key,
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
			.equippedCharacter = characterKey,
		},
	});
	Store::artifacts.insert({
		{++Store::lastArtifactId},
		Artifact::Instance{
			.key = {Store::lastArtifactId},
			.set = artifactData.key,
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
			.equippedCharacter = characterKey,
		},
	});
	Store::artifacts.insert({
		{++Store::lastArtifactId},
		Artifact::Instance{
			.key = {Store::lastArtifactId},
			.set = artifactData.key,
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
			.equippedCharacter = characterKey,
		},
	});

	character.getArtifactStats();

	return character;
}

[[nodiscard]] const Node::Instance &getNode(Character::Instance &character) {
	return character.loadout.character.data.data.nodes.burst.at(0);
}

static void formulaCalc(benchmark::State &state) {
	// static_assert(Stats::SheetLike<Stats::CharacterSheet>, "Character sheet must be SheetLike");
	// static_assert(Stats::SheetLike<Stats::WeaponSheet>, "Character sheet must be SheetLike");

	Weapon::initWeapons();
	Character::initCharacters();
	Artifact::initArtifacts();

	auto &character = getCharacter();

	const auto &node = getNode(character);

	std::random_device rd{};
	std::mt19937 mt(rd());
	std::uniform_int_distribution<unsigned short> levelGen(1, 90);

	Stats::Team team{};
	team.characters.at(0) = character;
	// team.characters.at(1) = character;
	// team.characters.at(2) = character;
	// team.characters.at(3) = character;

	Stats::Enemy enemy{};
	enemy.sheet.level.modifiers.at(0) = Formula::Constant(100.f);

	Formula::Context ctx{
		.source = character.loadout,
		.target = character.loadout,
		.team = team,
		.enemy = enemy,
	};

	for (auto _: state) {
		benchmark::DoNotOptimize(node.formula.eval(ctx));
	}
}
BENCHMARK(formulaCalc);

BENCHMARK_MAIN();