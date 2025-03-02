#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "store.hpp"
#include "utils/storageFolder.hpp"
#include "weapon/weapons.hpp"


#include "Ui/home/homePage.hpp"

#include "window.hpp"
#include <GLFW/glfw3.h>

#include "glaze/glaze.hpp"
#include "serialization/good/IGOOD.hpp"

int main() {
	auto file = std::ifstream("data.json");
	if (file.is_open()) {
		std::stringstream ss{};
		ss << file.rdbuf();
		Serialization::Good::IGOOD dst;
		auto d = glz::read<glz::opts{.error_on_unknown_keys = false}>(dst, ss.str());
		if (!d) {
			std::println("{}", dst.format);
		}
	}


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

	Window::run();

	::Store::saveToFile(fileSavePath);
}
