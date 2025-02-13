#include "artifact/instance.hpp"
#include "artifact/sets.hpp"// IWYU pragma: keep
#include "character/characters.hpp"
#include "src/formula/node.hpp"
#include "stats/team.hpp"
#include "store.hpp"
#include "weapon/weapons.hpp"
#include <random>


#include "benchmark/benchmark.h"

namespace {
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
		character.loadout.artifact.equipped = {.flower = artiKey1, .plume = artiKey2, .sands = artiKey3, .goblet = artiKey4, .circlet = artiKey5};

		Store::artifacts.insert({
			{Store::lastArtifactId},
			Artifact::Instance{
				.key = {Store::lastArtifactId++},
				.set = artifactData.key,
				.slot = Artifact::Slot::flower,
				.mainStat = Stat::hp,
				.subStats = {
					StatValue{.stat = Stat::cr, .value = 0.062f},
					StatValue{.stat = Stat::def_, .value = 0.058f},
					StatValue{.stat = Stat::cd, .value = 0.202f},
					StatValue{.stat = Stat::er, .value = 0.123f},
				},
				.level = 20,
				.equippedCharacter = characterKey,
			},
		});
		Store::artifacts.insert({
			{Store::lastArtifactId},
			Artifact::Instance{
				.key = {Store::lastArtifactId++},
				.set = artifactData.key,
				.slot = Artifact::Slot::plume,
				.mainStat = Stat::atk,
				.subStats = {
					StatValue{.stat = Stat::def, .value = 19.f},
					StatValue{.stat = Stat::hp, .value = 269.f},
					StatValue{.stat = Stat::cd, .value = 0.148f},
					StatValue{.stat = Stat::cr, .value = 0.132f},
				},
				.level = 20,
				.equippedCharacter = characterKey,
			},
		});
		Store::artifacts.insert({
			{Store::lastArtifactId},
			Artifact::Instance{
				.key = {Store::lastArtifactId++},
				.set = artifactData.key,
				.slot = Artifact::Slot::sands,
				.mainStat = Stat::em,
				.subStats = {
					StatValue{.stat = Stat::cr, .value = 0.101f},
					StatValue{.stat = Stat::er, .value = 0.104f},
					StatValue{.stat = Stat::cd, .value = 0.070f},
					StatValue{.stat = Stat::def_, .value = 0.139f},
				},
				.level = 20,
				.equippedCharacter = characterKey,
			},
		});
		Store::artifacts.insert({
			{Store::lastArtifactId},
			Artifact::Instance{
				.key = {Store::lastArtifactId++},
				.set = artifactData.key,
				.slot = Artifact::Slot::goblet,
				.mainStat = Stat::electroDmg,
				.subStats = {
					StatValue{.stat = Stat::atk_, .value = 0.152f},
					StatValue{.stat = Stat::def, .value = 37.f},
					StatValue{.stat = Stat::cr, .value = 0.078f},
					StatValue{.stat = Stat::cd, .value = 0.124f},
				},
				.level = 20,
				.equippedCharacter = characterKey,
			},
		});
		Store::artifacts.insert({
			{Store::lastArtifactId},
			Artifact::Instance{
				.key = {Store::lastArtifactId++},
				.set = artifactData.key,
				.slot = Artifact::Slot::circlet,
				.mainStat = Stat::cd,
				.subStats = {
					StatValue{.stat = Stat::atk_, .value = 0.058f},
					StatValue{.stat = Stat::cr, .value = 0.109f},
					StatValue{.stat = Stat::hp, .value = 598.0f},
					StatValue{.stat = Stat::em, .value = 47.f},
				},
				.level = 20,
				.equippedCharacter = characterKey,
			},
		});

		character.getArtifactStats();

		return character;
	}

	[[nodiscard]] const Node::Instance &getNode(Character::Instance &character) {
		return character.loadout.character.data.data.nodes.burst.at(0);
	}

	void formulaCalc(benchmark::State &state) {
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
		team.characters.at(0) = &character;
		team.characters.at(1) = &character;
		team.characters.at(2) = &character;
		team.characters.at(3) = &character;

		Stats::Enemy enemy{};
		enemy.sheet.level.constant = 100.f;

		Formula::Context ctx{
			.source = character.loadout,
			.target = character.loadout,
			.team = team,
			.enemy = enemy,
		};

		for (auto _: state) {
			benchmark::DoNotOptimize(node.formula.eval(ctx));
			// character.getArtifactStats();
		}
	}
}// namespace


BENCHMARK(formulaCalc);

BENCHMARK_MAIN();