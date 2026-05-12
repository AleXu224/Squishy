#include "agent/agents.hpp"
#include "core/app.hpp"
#include "disc/sets.hpp"
#include "engine/engines.hpp"
#include "utils/storageFolder.hpp"

#include "UI/home/homePage.hpp"
#include "store.hpp"

int main() {
	Engine::initEngines();
	Agent::initAgents();
	Disc::initDiscs();

	auto fileSavePath = Utils::getStorageFolder().value() / "Squishy" / "save_zzz.sqsh";
	::Store::loadFromFile(fileSavePath.string());

	using namespace squi;

	::Store::enemies.insert(
		{
			0,
			Enemy::Instance{
				.key = 0,
				.name = "Default enemy",
				.stats{
					.sheet{
						.level{.constant = 70.f},
						.baseDef{.constant = 60.f},
					},
				},
			},
		}
	);

	auto systemTheme = Theme::getSystemAccentColor();

	App app{
		.windowOptions{
			.name = "Squishy",
			.width = ::Store::windowWidth,
			.height = ::Store::windowHeight,
			.maximized = ::Store::maximized,
		},
		.child = UI::HomePage{
			.key = GlobalKey{},
		},
		.theme = Theme{
			.accent = systemTheme.value_or(Theme{}.accent),
		},
	};
	app.initialize();
	App::runAllWindows();

	::Store::saveToFile(fileSavePath.string());
}