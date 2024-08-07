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
			data.stats.character.sheet.em.artifactModifiers.emplace_back([](const Stats::Sheet &) {
				return 80.f;
			});
		},
		.fourPcModsSetup = [](Artifact::Set::ModsSetup data) {
			data.stats.character.sheet.atk_.artifactModifiers.emplace_back([](const Stats::Sheet &stats) {
				auto cond = std::get<Conditional::ValueList>(stats.artifact.conditionals.at("gildedSameElement"));
				return 0.14f * cond.getValue().value_or(0.f);
			});
			data.stats.character.sheet.em.artifactModifiers.emplace_back([](const Stats::Sheet &stats) {
				auto cond = std::get<Conditional::ValueList>(stats.artifact.conditionals.at("gildedOtherElement"));
				return 50.f * cond.getValue().value_or(0.f);
			});
		},
		.nodeSetup = [](Artifact::Set::NodeSetup data) -> Node::List {
			return {};
		},
	};
}// namespace Artifact::Sets
