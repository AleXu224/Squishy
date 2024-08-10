#include "characterStatInspector.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "gestureDetector.hpp"
#include "scrollableFrame.hpp"
#include "stack.hpp"
#include "store.hpp"
#include "text.hpp"

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
				.child = Box{
					.widget{
						.height = Size::Shrink,
						.sizeConstraints{
							.maxWidth = 1000.f,
						},
					},
					.color{0x000000FF},
					.child = ScrollableFrame{
						.children{Masonry{
							.widget{
								.height = Size::Shrink,
							},
							.spacing = 4.f,
							.columnCount = Masonry::MinSize{256.f},
							.children = [&character = Store::characters.at(characterKey)]() {
								Children ret{};

								auto stats = {
									Stat::hp,
									Stat::hp_,
									Stat::baseHp,
									Stat::atk,
									Stat::atk_,
									Stat::baseAtk,
									Stat::additionalAtk,
									Stat::def,
									Stat::def_,
									Stat::baseDef,
									Stat::er,
									Stat::em,
									Stat::cr,
									Stat::cd,
									Stat::hb,
									Stat::pyroDmg,
									Stat::hydroDmg,
									Stat::cryoDmg,
									Stat::electroDmg,
									Stat::dendroDmg,
									Stat::anemoDmg,
									Stat::geoDmg,
									Stat::physicalDmg,
									Stat::allDmg,
								};

								for (auto stat: stats) {
									auto statCharacter = character.stats.character.sheet.fromStat(stat);
									auto statArtifact = character.stats.artifact.sheet.fromStat(stat);
									auto statWeapon = character.stats.weapon.sheet.fromStat(stat);
									Children ret2{};

									if (!statCharacter.modifiers.empty()) ret2.emplace_back(Text{.text = "CharacterStats", .fontSize = 16.f});
									for (auto &modifier: statCharacter.modifiers) {
										ret2.emplace_back(StatDisplay{
											.isTransparent = true,
											.stat{
												.stat = stat,
												.value = modifier.eval(character.stats),
											},
										});
									};
									if (!statWeapon.modifiers.empty()) ret2.emplace_back(Text{.text = "WeaponStats", .fontSize = 16.f});
									for (auto &modifier: statWeapon.modifiers) {
										ret2.emplace_back(StatDisplay{
											.isTransparent = true,
											.stat{
												.stat = stat,
												.value = modifier.eval(character.stats),
											},
										});
									};
									if (!statArtifact.modifiers.empty()) ret2.emplace_back(Text{.text = "ArtifactStats", .fontSize = 16.f});
									for (auto &modifier: statArtifact.modifiers) {
										ret2.emplace_back(StatDisplay{
											.isTransparent = true,
											.stat{
												.stat = stat,
												.value = modifier.eval(character.stats),
											},
										});
									};

									if (!ret2.empty()) {
										ret.emplace_back(Card{
											.widget{.height = Size::Shrink, .padding = 4.f},
											.child = Column{.spacing = 4.f, .children = ret2},
										});
									}
								}

								return ret;
							}(),
						}},
					},
				},
			},
		},
	};
}
