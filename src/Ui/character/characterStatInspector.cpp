#include "characterStatInspector.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "align.hpp"
#include "box.hpp"
#include "gestureDetector.hpp"
#include "scrollableFrame.hpp"
#include "stack.hpp"
#include "store.hpp"

using namespace squi;

UI::CharacterStatInspector::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Stack{
		.children{
			GestureDetector{
				.onClick = [](GestureDetector::Event event) {
					event.widget.state.parent->deleteLater();
				},
				.child = Box{
					.color = Color{0.f, 0.f, 0.f, 0.2f},
				},
			},
			Align{
				.child = Card{
					.widget{
						.height = Size::Shrink,
						.sizeConstraints{
							.maxWidth = 256.f,
						},
					},
					.child = ScrollableFrame{
						.children = [&character = Store::characters.at(characterKey)]() {
							Children ret{};

							auto statsCharacter = {&Stats::CharacterSheet::hp, &Stats::CharacterSheet::hp_, &Stats::CharacterSheet::atk, &Stats::CharacterSheet::atk_, &Stats::CharacterSheet::additionalAtk, &Stats::CharacterSheet::def, &Stats::CharacterSheet::def_, &Stats::CharacterSheet::er, &Stats::CharacterSheet::em, &Stats::CharacterSheet::cr, &Stats::CharacterSheet::cd, &Stats::CharacterSheet::hb};
							auto statsArtifact = {&Stats::ArtifactSheet::hp, &Stats::ArtifactSheet::hp_, &Stats::ArtifactSheet::atk, &Stats::ArtifactSheet::atk_, &Stats::ArtifactSheet::additionalAtk, &Stats::ArtifactSheet::def, &Stats::ArtifactSheet::def_, &Stats::ArtifactSheet::er, &Stats::ArtifactSheet::em, &Stats::ArtifactSheet::cr, &Stats::ArtifactSheet::cd, &Stats::ArtifactSheet::hb};
							auto statsWeapon = {&Stats::WeaponSheet::hp, &Stats::WeaponSheet::hp_, &Stats::WeaponSheet::atk, &Stats::WeaponSheet::atk_, &Stats::WeaponSheet::additionalAtk, &Stats::WeaponSheet::def, &Stats::WeaponSheet::def_, &Stats::WeaponSheet::er, &Stats::WeaponSheet::em, &Stats::WeaponSheet::cr, &Stats::WeaponSheet::cd, &Stats::WeaponSheet::hb};
							auto stats2 = {Stat::hp, Stat::hp_, Stat::atk, Stat::atk_, Stat::additionalAtk, Stat::def, Stat::def_, Stat::er, Stat::em, Stat::cr, Stat::cd, Stat::hb};

							for (auto [statPtrCharacter, statPtrArtifact, statPtrWeapon, statEn]: std::views::zip(statsCharacter, statsArtifact, statsWeapon, stats2)) {
								auto statCharacter = std::invoke(statPtrCharacter, character.stats.character.sheet);
								auto statArtifact = std::invoke(statPtrArtifact, character.stats.artifact.sheet);
								auto statWeapon = std::invoke(statPtrWeapon, character.stats.weapon.sheet);

								for (auto &modifier: statCharacter.modifiers) {
									ret.emplace_back(StatDisplay{
										.isTransparent = true,
										.stat{
											.stat = statEn,
											.value = modifier(character.stats),
										},
									});
								};
								for (auto &modifier: statWeapon.modifiers) {
									ret.emplace_back(StatDisplay{
										.isTransparent = false,
										.stat{
											.stat = statEn,
											.value = modifier(character.stats),
										},
									});
								};
								for (auto &modifier: statArtifact.modifiers) {
									ret.emplace_back(StatDisplay{
										.isTransparent = false,
										.stat{
											.stat = statEn,
											.value = modifier(character.stats),
										},
									});
								};
							}

							return ret;
						}(),
					},
				},
			},
		},
	};
}
