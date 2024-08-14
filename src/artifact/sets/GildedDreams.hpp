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
			Stats::addModifierArtifact(data.stats.sheet.em, Formula::ConstantFlat(80.f));
		},
		.fourPcModsSetup = [](Artifact::Set::ModsSetup data) {
			Stats::addModifierArtifact(data.stats.sheet.atk_, 0.14f * Formula::ConditionalValue(Conditional::Location::artifact, "gildedSameElement"));
			Stats::addModifierArtifact(data.stats.sheet.em, Formula::ConstantFlat(50.f) * Formula::ConditionalValue(Conditional::Location::artifact, "gildedOtherElement"));
		},
		.nodeSetup = []() -> Node::ArtifactList {
			return {};
		},
	};
}// namespace Artifact::Sets
