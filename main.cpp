#include "UI/Screens/CharacterScreen.hpp"
#include "UI/TopNav.hpp"
#include "Weapons/Staff_of_the_Scarlet_Sands.hpp"
#include "character.hpp"
#include "characterStore.hpp"
#include "window.hpp"
#include <memory>
#include <print>

int main(int, char **) {
	using namespace squi;
	using namespace Squishy;

	for (const auto &charData: CharacterStore::charactersData) {
		CharacterStore::characters.emplace_back(std::make_unique<Character>(charData.second, Staff_of_the_Scarlet_Sands));
	}

	Window window;
	window.addChild(TopNav{
		.tabs{
			{"Characters", CharacterScreen{}},
		},
	});
	window.run();
}
