#include "UI/Screens/ArtifactsScreen.hpp"
#include "UI/Screens/CharacterScreen.hpp"
#include "UI/TopNav.hpp"
#include "Weapons/Staff_of_the_Scarlet_Sands.hpp"
#include "artifact.hpp"
#include "character.hpp"
#include "characterStore.hpp"
#include "window.hpp"
#include <memory>
#include <print>

int main(int, char **) {
	using namespace squi;
	using namespace Squishy;

	for (const auto &charData: Store::Data::characters) {
		Store::characters.emplace_back(std::make_unique<Character>(charData, Staff_of_the_Scarlet_Sands));
	}

	Store::artifacts.emplace_back(std::make_shared<Artifact>(Artifact{
		.level = 16,
		.rarity = 4,
		.slot = ArtifactSlot::Flower,
		.mainStat{
			.stat = ArtifactStat::HP,
			.value = 4780.f,
		},
		.subStats{
			Artifact::SubStat{
				.stat = ArtifactStat::HP,
				.value = 4780.f,
			},
			Artifact::SubStat{
				.stat = ArtifactStat::EM,
				.value = 60.f,
			},
			Artifact::SubStat{
				.stat = ArtifactStat::ER,
				.value = 58.f,
			},
			Artifact::SubStat{
				.stat = ArtifactStat::CritRate,
				.value = 3.7f,
			},
		},
	}));

	Window window;
	window.addChild(TopNav{
		.tabs{
			{"Artifacts", ArtifactsScreen{}},
			{"Characters", CharacterScreen{}},
		},
	});
	window.run();
}
