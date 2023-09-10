#include "UI/Screens/ArtifactsScreen.hpp"
#include "UI/Card.hpp"
#include "UI/Grid.hpp"
#include "UI/Rarity.hpp"
#include "artifact.hpp"
#include "box.hpp"
#include "characterStore.hpp"
#include "column.hpp"
#include "container.hpp"
#include "fontIcon.hpp"
#include "image.hpp"
#include "margin.hpp"
#include "row.hpp"
#include "stat.hpp"
#include "text.hpp"
#include <format>
#include <functional>
#include <memory>

using namespace squi;
using namespace Squishy;

struct ArtifactCardHeader {
	// Args
	std::shared_ptr<Artifact> artifact;

	operator Child() const {
		return Box{
			.widget{
				.height = 128.f,
			},
			.color = qualityColors.at(artifact->rarity),
			.child = Row{
				.alignment = Row::Alignment::center,
				.children{
					Column{
						.widget{
							.padding = 8.f,
						},
						.children{
							Box{
								.widget{
									.width = Size::Shrink,
									.height = Size::Shrink,
									.padding = 2.f,
								},
								.color = Color::RGBA255(32, 32, 32),
								.borderRadius = 2.f,
								.child = Text{
									.text{std::format("+{}", artifact->level)},
								},
							},
							Container{},
							Text{
								.text{ArtifactStatToString(artifact->mainStat.stat)},
								.fontSize = 12.f,
								.lineWrap = true,
							},
							Text{
								.text{std::format("{}{}", artifact->mainStat.value, ArtifactStatIsPercentage(artifact->mainStat.stat) ? "%" : "")},
								.fontSize = 32.f,
							},
							Row{
								.widget{
									.height = Size::Shrink,
								},
								.spacing = 4.f,
								.children = std::invoke([&]() -> Children {
									Children ret{};
									ret.reserve(artifact->rarity);
									for (uint32_t i = 0; i < artifact->rarity; i++) {
										ret.emplace_back(FontIcon{.icon = 0xE735, .size = 20.f, .color = Color::RGBA255(255, 204, 50)});
									}
									return ret;
								}),
							},
						},
					},
					Image{
						.widget{
							.width = 128.f,
							.height = 128.f,
						},
						.fit = Image::Fit::contain,
						.image = Image::Data::fromUrlAsync("https://static.wikia.nocookie.net/gensin-impact/images/0/0c/Item_Dreaming_Steelbloom.png/revision/latest?cb=20220826125336"),
					},
				},
			},
		};
	};
};

struct ArtifactCardSubstat {
	// Args
	Artifact::SubStat &substat;
	bool isTransparent = false;

	operator Child() const {
		return Box{
			.widget{
				.height = Size::Shrink,
				.sizeConstraints{
					.minHeight = 36.f,
				},
				.padding = Padding{12.f, 0.f},
			},
			.color = isTransparent ? Color::HEX(0x00000000) : Color::RGBA(1, 1, 1, 0.04),
			.borderRadius = 4.f,
			.child = Row{
				.alignment = Row::Alignment::center,
				.children{
					Text{
						.text{ArtifactStatToString(substat.stat)},
						.lineWrap = true,
					},
					Text{
						.text{std::format("+{}{}", substat.value, ArtifactStatIsPercentage(substat.stat) ? "%" : "")},
					},
				},
			},
		};
	}
};

struct ArtifactCardSubstats {
	// Args
	std::shared_ptr<Artifact> artifact;

	struct Storage {
		// Data
	};

	operator Child() const {
		auto storage = std::make_shared<Storage>();

		return Column{
			.widget{
				.height = Size::Shrink,
				.padding = 4.f,
			},
			.children{
				ArtifactCardSubstat{.substat = artifact->subStats[0], .isTransparent = true},
				ArtifactCardSubstat{.substat = artifact->subStats[1]},
				ArtifactCardSubstat{.substat = artifact->subStats[2], .isTransparent = true},
				ArtifactCardSubstat{.substat = artifact->subStats[3]},
			},
		};
	}
};

struct ArtifactCard {
	// Args
	Widget::Args widget;
	std::shared_ptr<Artifact> artifact;

	struct Storage {
		// Data
		std::shared_ptr<Artifact> artifact;
	};

	operator Child() const {
		auto storage = std::make_shared<Storage>(artifact);

		return Card{
			.widget{widget.withDefaultHeight(Size::Shrink)},
			.child = Column{
				.widget{
					.height = Size::Shrink,
				},
				.children{
					ArtifactCardHeader{.artifact = artifact},
					ArtifactCardSubstats{.artifact = artifact},
				},
			},
		};
	}
};

Squishy::ArtifactsScreen::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return Grid{
		.widget{widget.withDefaultPadding(12.f)},
		.spacing = 12.f,
		.minChildWidth = 250.f,
		.children = std::invoke([&]() -> Children {
			Children ret{};
			for (auto &artifact: Store::artifacts) {
				ret.emplace_back(ArtifactCard{
					.artifact = artifact,
				});
			}
			return ret;
		}),
	};
}
