#include "optimizationResult.hpp"

#include "Ui/artifact/artifactCard.hpp"
#include "artifact/sets.hpp"
#include "store.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/expander.hpp"
#include "widgets/image.hpp"
#include "widgets/row.hpp"


using namespace squi;

[[nodiscard]] squi::core::Child UI::OptimizationResult::build(const Element &) const {
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
		.title = std::format("Result #{}", entryIndex),
		.subtitle = std::format("Score: {}", solution.score),
		.action = Button{
			.theme = Button::Theme::Accent(),
			.onClick = [this]() {
				auto &character = ::Store::characters.at(characterKey);

				for (const auto &slot: Artifact::slots) {
					auto &artifactKey = solution.artifacts.fromSlot(slot);

					if (!artifactKey && character.state.loadout().artifact.getSlotted().fromSlot(slot)) {
						character.state.loadout().artifact.getSlotted().fromSlot(slot).clear();
					}

					auto &artifact = ::Store::artifacts.at(artifactKey);
					artifact.equipOn(characterKey, character.state.loadoutIndex);
				}
				character.state.loadout().artifact.refreshStats();
				character.updateEvent.notify();
			},
			.child = "Equip",
		},
		.content = Column{
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
