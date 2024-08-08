#pragma once

#include "artifact/set.hpp"

namespace Artifact::Sets {
	const inline Artifact::Set gildedDreams{
		.key = 15026,
		.name = "Gilded Dreams",
		.condsSetup = [](Artifact::Set::CondsSetup data) -> Conditional::ArtifactList {
			return {
				// FIXME: this should be done automagically
				Conditional::ValueList{
					.key = "gildedSameElement",
					.prefix = "Characters with the same Elemental Type",
					.values = {1, 2, 3},
				},
				Conditional::ValueList{
					.key = "gildedOtherElement",
					.prefix = "Characters with a different Elemental Type",
					.values = {1, 2, 3},
				},
			};
		},
		.twoPcModsSetup = [](Artifact::Set::ModsSetup data) {
			Stats::addModifier(data.stats.sheet.em, Stats::SV::Constant(80.f));
		},
		.fourPcModsSetup = [](Artifact::Set::ModsSetup data) {
			Stats::addModifier(data.stats.sheet.atk_, [](const Stats::Sheet &stats) {
				return 0.14f * Conditional::getFloat(stats.artifact.conditionals, "gildedSameElement");
			});
			Stats::addModifier(data.stats.sheet.em, [](const Stats::Sheet &stats) {
				return 50.f * Conditional::getFloat(stats.artifact.conditionals, "gildedOtherElement");
			});
		},
		.nodeSetup = []() -> Node::ArtifactList {
			return {};
		},
	};
}// namespace Artifact::Sets
