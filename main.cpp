#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "store.hpp"
#include "utils/storageFolder.hpp"
#include "weapon/weapons.hpp"


#include "Ui/home/homePage.hpp"

#include "window.hpp"
#include <GLFW/glfw3.h>

#include "glaze/glaze.hpp"// IWYU pragma: keep
#include "serialization/good/IGOOD.hpp"

int main() {
	Weapon::initWeapons();
	Character::initCharacters();
	Artifact::initArtifacts();

	auto fileSavePath = Utils::getStorageFolder().value() / "Squishy" / "save.sqsh";
	::Store::loadFromFile(fileSavePath.string());

	// auto seriz = ::Store::saveToGOOD();
	auto file = std::ifstream("data.json");
	if (file.is_open()) {
		std::stringstream ss{};
		ss << file.rdbuf();
		Serialization::Good::IGOOD dst;
		auto d = glz::read<glz::opts{.error_on_unknown_keys = false}>(dst, ss.str());
		if (!d) {
			::Store::loadFromGOOD(dst);
		}
	}

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

	using namespace squi;
	Window window{};
	glfwSetWindowTitle(window.engine.instance.window.ptr, "Squishy");
	window.addChild(UI::homePage{});

	Window::run();

	::Store::saveToFile(fileSavePath.string());
}
