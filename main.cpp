#include "artifact/instance.hpp"
#include "artifact/sets.hpp"// IWYU pragma: keep
#include "artifact/sets/GildedDreams.hpp"
#include "character/characters.hpp"
#include "character/characters/Cyno.hpp"
#include "store.hpp"
#include "weapon/weapons/StaffOfTheScarletSands.hpp"


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

	// auto attacknode = Character::Datas::cyno.nodeSetup().normal.at(3);
	// if (attacknode.modifier) attacknode.modifier(Character::Datas::cyno, character.stats);
	// character.stats.sheet.print();
	// auto cr = character.stats.sheet.getStat(Stat::cr);
	// std::println("Cr: {}", cr);
	// float val = attacknode.getValue(Character::Datas::cyno, character.stats);
	// std::println("{}", val);

	// fmt::println("{}", Formula::Constant(1.f).print(character.stats));
	// fmt::println("{}", Formula::ConditionalValue(Conditional::Location::burst, "burstActive").print(character.stats));
	// fmt::println("{}", Formula::Conditional(Conditional::Location::burst, "burstActive", Formula::Constant(1.f)).print(character.stats));

	// fmt::println("{}", Formula::_isSheetMemberPercentage(&Stats::WeaponSheet::baseAtk));
	// constexpr auto stat = &Stats::WeaponSheet::baseAtk;
	// Formula::Node nd = Formula::StatPtr(stat);
	// fmt::println("{}", nd.print(character.stats));

	using namespace squi;
	Window window{};
	glfwSetWindowTitle(window.engine.instance.window.ptr, "Squishy");
	// window.setChild(new TopNav(TopNavArgs{
	// 	.tabs{
	// 		TopNavTab{
	// 			.name{"Artifacts"},
	// 			.child = new ArtifactPage(ArtifactPageArgs{}),
	// 		},
	// 		TopNavTab{
	// 			.name{"Characters"},
	// 			.child = new CharacterPage(CharacterPageArgs{}),
	// 		},
	// 	},
	// }));
	window.addChild(UI::homePage{});
	window.run();

	// screen.child = std::make_shared<TopNav::NavigationView>(TopNav::NavigationViewArgs{
	// 	.pages {
	// 		TopNav::NavBarPage{
	// 			.name{"Artifacts"},
	// 			.child = std::make_shared<ArtifactPage>()
	// 		},
	// 		TopNav::NavBarPage{
	// 			.name{"Characters"},
	// 			.child = std::make_shared<CharacterPage>()
	// 		}
	// 	}
	// });
	// screen.run();
}
