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

							auto stats = {&Stats::CharacterSheet::hp, &Stats::CharacterSheet::hp_, &Stats::CharacterSheet::atk, &Stats::CharacterSheet::atk_, &Stats::CharacterSheet::additionalAtk, &Stats::CharacterSheet::def, &Stats::CharacterSheet::def_, &Stats::CharacterSheet::er, &Stats::CharacterSheet::em, &Stats::CharacterSheet::cr, &Stats::CharacterSheet::cd, &Stats::CharacterSheet::hb};
							auto stats2 = {Stat::hp, Stat::hp_, Stat::atk, Stat::atk_, Stat::additionalAtk, Stat::def, Stat::def_, Stat::er, Stat::em, Stat::cr, Stat::cd, Stat::hb};

							for (auto [statPtr, statEn]: std::views::zip(stats, stats2)) {
								auto stat = std::invoke(statPtr, character.stats.character.sheet);

								for (auto [modifier]: std::views::zip(stat.modifiers)) {
									ret.emplace_back(StatDisplay{
										.isTransparent = true,
										.stat{
											.stat = statEn,
											.value = modifier(character.stats),
										},
									});
								};
								for (auto [modifier]: std::views::zip(stat.artifactModifiers)) {
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
