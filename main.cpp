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
	// using T = Stats::Sheet::_Stat;
	// T sheet{};
	// auto a = std::views::transform(
	// 	std::views::cartesian_product(
	// 		Stats::getSheetAttackSourceMembers<T>(),
	// 		T::_SkillValue::getMembers()
	// 	),
	// 	[&](const auto &val) {
	// 		return std::invoke(std::get<1>(val), std::invoke(std::get<0>(val), sheet));
	// 	}
	// );


	// static_assert(Stats::SheetLike<Stats::CharacterSheet>, "Character sheet must be SheetLike");
	// static_assert(Stats::SheetLike<Stats::WeaponSheet>, "Character sheet must be SheetLike");

	Weapon::initWeapons();
	Character::initCharacters();
	Artifact::initArtifacts();
	auto weaponData = Weapon::list.begin()->second;
	auto characterData = Character::list.begin()->second;
	auto artifactData = Artifact::sets.begin()->second;
	auto &weapon = Store::weapons.insert({weaponData.key, Weapon::Instance(weaponData.key)}).first->second;
	weapon.stats.sheet.level = 90;
	weapon.stats.sheet.ascension = 6;
	weapon.stats.sheet.refinement = 1;
	auto &character = Store::characters.insert({characterData.key, Character::Instance(characterData.key, weaponData.key)}).first->second;
	character.stats.character.sheet.level = 90;
	character.stats.character.sheet.ascension = 6;
	character.stats.character.sheet.talents.burst = 9;
	character.stats.character.sheet.talents.skill = 9;
	character.stats.character.sheet.constellation = 0;
	character.stats.artifact.equipped = {1, 2, 3, 4, 5};

	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
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
			.equippedCharacter = characterData.key,
		},
	});
	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
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
			.equippedCharacter = characterData.key,
		},
	});
	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
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
			.equippedCharacter = characterData.key,
		},
	});
	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
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
			.equippedCharacter = characterData.key,
		},
	});
	Store::artifacts.insert({
		++Store::lastId,
		Artifact::Instance{
			.key = Store::lastId,
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
			.equippedCharacter = characterData.key,
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
