#include "Ui/weapon/weaponSelector.hpp"
#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "store.hpp"
#include "utils/storageFolder.hpp"
#include "weapon/weapons.hpp"


#include "Ui/home/homePage.hpp"

#include "window.hpp"
#include <GLFW/glfw3.h>

int main() {
	Weapon::initWeapons();
	Character::initCharacters();
	Artifact::initArtifacts();

	auto fileSavePath = Utils::getStorageFolder().value() / "Squishy" / "save.sqsh";
	::Store::loadFromFile(fileSavePath);

	auto enemy = Store::enemies.insert(
		{
			0,
			Enemy::Instance{
				.key = 0,
				.name = "Default enemy",
			},
		}
	);

	enemy.first->second.stats.sheet.level.constant = 100.f;

	Store::teams.insert({
		{0},
		Team::Instance{},
	});

	using namespace squi;
	Window window{};
	glfwSetWindowTitle(window.engine.instance.window.ptr, "Squishy");
	window.addChild(UI::homePage{});

	Window window2{};
	window2.addChild(UI::WeaponSelector{
		.onSelect = [](Weapon::DataKey key){
			std::println("{}", key.key);
		},
	});

	Window::run();

	::Store::saveToFile(fileSavePath);
}
