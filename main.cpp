#include "artifact/instance.hpp"
#include "artifact/sets.hpp"// IWYU pragma: keep
#include "character/characters.hpp"
#include "formula/constant.hpp"
#include "store.hpp"


#include "Ui/home/homePage.hpp"

#include "window.hpp"
#include <GLFW/glfw3.h>

// #include "random"

//#include "benchmark/benchmark.h"
//benchmark::State& state
int main() {
	Weapon::initWeapons();
	Character::initCharacters();
	Artifact::initArtifacts();
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
		{++Store::lastId},
		Artifact::Instance{
			.key = {Store::lastId},
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
		{++Store::lastId},
		Artifact::Instance{
			.key = {Store::lastId},
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
		{++Store::lastId},
		Artifact::Instance{
			.key = {Store::lastId},
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
		{++Store::lastId},
		Artifact::Instance{
			.key = {Store::lastId},
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
		{++Store::lastId},
		Artifact::Instance{
			.key = {Store::lastId},
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

	auto team = Store::teams.insert({
		0,
		Team::Instance{
			.key = 0,
			.name = "Test team",
		},
	});
	team.first->second.stats.characters.at(0) = character;

	auto enemy = Store::enemies.insert(
		{
			0,
			Enemy::Instance{
				.key = 0,
				.name = "Default enemy",
			},
		}
	);

	enemy.first->second.stats.sheet.level.modifiers.at(0) = Formula::ConstantFlat(100.f);

	character.getArtifactStats();

	using namespace squi;
	Window window{};
	glfwSetWindowTitle(window.engine.instance.window.ptr, "Squishy");
	window.addChild(UI::homePage{});
	window.run();
}
