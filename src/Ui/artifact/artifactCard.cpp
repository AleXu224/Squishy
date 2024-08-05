#include "artifactCard.hpp"
#include "Ui/rarityToColor.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "align.hpp"
#include "artifact/sets.hpp"
#include "box.hpp"
#include "column.hpp"
#include "image.hpp"
#include "stack.hpp"
#include "text.hpp"


using namespace squi;

struct ArtifactHeader {
	// Args
	Artifact::SetKey set;
	Artifact::Slot slot;
	StatValue mainStat;

	struct Storage {
		// Data
	};

	operator squi::Child() const {
		auto storage = std::make_shared<Storage>();

		return Box{
			.widget{
				.height = 64.f,
			},
			.color = Utils::rarityToColor.at(Rarity::fiveStar),
			.borderRadius{7.f, 7.f, 0.f, 0.f},
			.child = Stack{
				.children{
					Align{
						.xAlign = 1.f,
						.child = Image{
							.fit = squi::Image::Fit::contain,
							.image = Image::Data::fromFileAsync(std::format("assets/Artifacts/{}/{}.png", Artifact::sets.at(set).name, Utils::Stringify(slot))),
						},
					},
					Align{
						.xAlign = 0.f,
						.child = Column{
							.widget{
								.height = Size::Shrink,
								.padding = Padding{12.f, 0.f},
							},
							.children{
								Text{
									.text = Utils::Stringify(mainStat.stat),
								},
								Text{
									.text = Utils::Stringify(mainStat),
									.fontSize = 24.f,
								},
							},
						},
					},
				},
			},
		};
	}
};

UI::ArtifactCard::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Card{
		.widget{
			.padding = Padding{1.f},
		},
		.child = Column{
			.children{
				ArtifactHeader{
					.set = artifact.set,
					.slot = artifact.slot,
					.mainStat{
						.stat = artifact.mainStat,
						.value = Stats::Values::mainStat.at(artifact.mainStat).at(artifact.level),
					},
				},
				Column{
					.widget{
						.onInit = [artifact = artifact](Widget &w) {
							auto trueFalse = {false, true};
							auto trueFalseInf = std::views::join(std::views::repeat(trueFalse));

							for (const auto &[transparent, substat]: std::views::zip(trueFalseInf, artifact.subStats)) {
								w.addChild(StatDisplay{
									.isTransparent = transparent,
									.stat = substat,
								});
							}
						},
					},
				},
			},
		},
	};
}
