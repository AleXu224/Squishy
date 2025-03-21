#include "optimizationResult.hpp"

#include "Ui/artifact/artifactCard.hpp"
#include "artifact/sets.hpp"
#include "box.hpp"
#include "button.hpp"
#include "column.hpp"
#include "expander.hpp"
#include "image.hpp"
#include "row.hpp"
#include "store.hpp"


using namespace squi;

UI::OptimizationResult::operator squi::Child() const {
	return Expander{
		.widget = widget,
		.icon = Row{
			.widget{
				.width = Size::Shrink,
			},
			.spacing = 4.f,
			.children = [&]() {
				Children ret;
				for (const auto &slot: Artifact::slots) {
					auto &artifact = solution.artifacts.fromSlot(slot);
					ret.emplace_back(Box{
						.color = Color::css(0xffffff, 0.2f),
						.borderRadius = 4.f,
						.child = Image{
							.fit = squi::Image::Fit::contain,
							.image = ImageProvider::fromFile(std::format("assets/Artifacts/{}/{}.png", Artifact::sets.at(::Store::artifacts.at(artifact).set).name, Utils::Stringify(slot))),
						},
					});
				}
				return ret;
			}(),
		},
		.heading = std::format("Result #{}", entryIndex),
		.caption = std::format("Score: {}", solution.score),
		.actions{
			Button{
				.text = "Equip",
				.style = ButtonStyle::Accent(),
				.onClick = [characterKey = characterKey, solution = solution](GestureDetector::Event event) {
					auto &character = ::Store::characters.at(characterKey);
					for (const auto &slot: Artifact::slots) {
						auto &artifactKey = solution.artifacts.fromSlot(slot);
						if (!artifactKey && character.loadout.artifact.equipped.fromSlot(slot)) {
							::Store::artifacts.at(character.loadout.artifact.equipped.fromSlot(slot)).unequip();
						}
						auto &artifact = ::Store::artifacts.at(artifactKey);
						artifact.equipOn(characterKey);
					}
					character.loadout.artifact.refreshStats();
					character.updateEvent.notify();
				},
			},
		},
		.expandedContent = Column{
			.widget{
				.padding = 4.f,
			},
			.children{
				Row{
					.spacing = 4.f,
					.children = [solution = solution]() {
						Children ret;
						for (const auto &slot: Artifact::slots) {
							auto &key = solution.artifacts.fromSlot(slot);
							if (!key) continue;
							ret.emplace_back(ArtifactCard{
								.artifact = ::Store::artifacts.at(key),
								.actions = ArtifactCard::Actions::showcase,
							});
						}
						return ret;
					}(),
				},
			},
		},
	};
}
