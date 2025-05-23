#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "store.hpp"
#include "utils/storageFolder.hpp"
#include "weapon/weapons.hpp"

#include "Ui/home/homePage.hpp"

#include "theme.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>


int main() {
	Weapon::initWeapons();
	Character::initCharacters();
	Artifact::initArtifacts();

	auto fileSavePath = Utils::getStorageFolder().value() / "Squishy" / "save.sqsh";
	::Store::loadFromFile(fileSavePath.string());

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

	// auto &cyno = Store::characters.at({3});
	// auto &team = Store::teams.at({2});

	// auto ctx = Formula::Context{
	// 	.source = cyno.state,
	// 	.active = cyno.state,
	// 	.team = team.stats,
	// 	.enemy = enemy.first->second.stats,
	// };

	// auto source = Combo::Source::Character{.key{cyno.dataKey}, .slot = Node::CharacterSlot::burst, .index = 0};
	// auto resolved = source.resolve({});
	// auto compiled = resolved.formula.compile(ctx);
	// std::println("{}", compiled.print());
	// std::println("{}", compiled.eval(ctx));
	// std::println("{}", resolved.formula.eval(ctx));

	using namespace squi;

	auto systemTheme = ThemeManager::getSystemAccentColor();
	auto _ = ThemeManager::pushTheme(Theme{
		.accent = systemTheme.value_or(ThemeManager::getTheme().accent),
	});

	Window window{WindowOptions{
		.name = "Squishy",
		.width = ::Store::windowWidth,
		.height = ::Store::windowHeight,
		.maximized = ::Store::maximized,
	}};
	window.setMaximizeCallback([](bool maximized) {
		::Store::maximized = maximized;
	});
	window.setResizeCallback([](uint32_t width, uint32_t height) {
		if (!::Store::maximized) {
			::Store::windowWidth = width;
			::Store::windowHeight = height;
		}
	});
	window.addChild(UI::homePage{});

	Window::run();

	::Store::saveToFile(fileSavePath.string());
}
