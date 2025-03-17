#include "optimize.hpp"

#include "artifactFilter.hpp"
#include "store.hpp"

#include "bnb.hpp"
#include "chrono"
#include "ranges"
#include "solution.hpp"
#include <execution>


Optimization::Solutions Optimization::Optimization::optimize() const {
	auto start_filterGen = std::chrono::high_resolution_clock::now();

	std::vector<ArtifactFilter> filters{};

	// 4pc filters
	for (const auto &key: enabledFourPiece) {
		ArtifactFilter pattern{};
		auto &set = Artifact::sets.at(key);
		pattern.bonus1.emplace(Stats::ArtifactBonus{
			.setPtr = set,
			.bonusPtr = set.data.twoPc,
		});
		pattern.bonus2.emplace(Stats::ArtifactBonus{
			.setPtr = set,
			.bonusPtr = set.data.fourPc,
		});
		for (auto &entry: pattern.filters) {
			entry.set = key;
		}
		filters.emplace_back(pattern);

		for (size_t i = 0; i < 5; i++) {
			auto ret = pattern;
			ret.filters.at(i).set = std::nullopt;
			ret.filters.at(i).notSet = key;
			filters.emplace_back(ret);
		}
	}

	// 2pc filters
	for (auto it = enabledTwoPiece.begin(); it != enabledTwoPiece.end(); it++) {
		auto key = *it;
		auto &set = Artifact::sets.at(key);
		for (size_t i = 0; i < 5; i++) {
			for (size_t j = i + 1; j < 5; j++) {
				ArtifactFilter ret{};
				ret.bonus1.emplace(Stats::ArtifactBonus{
					.setPtr = set,
					.bonusPtr = set.data.twoPc,
				});
				ret.filters.at(i).set = key;
				ret.filters.at(j).set = key;
				if (threeRainbow) {
					auto retCopy = ret;
					std::vector<ArtifactSlotFilter *> otherSlots{};
					for (size_t k = 0; k < 5; k++) {
						if (k == i || k == j) continue;
						otherSlots.emplace_back(&retCopy.filters.at(k));
						retCopy.filters.at(k).notSet = key;
					}
					for (auto &slot: otherSlots) {
						slot->notSet = {};
						filters.emplace_back(retCopy);
						slot->notSet = key;
					}
				}

				// 2pc2pc
				for (auto it2 = std::next(it); it2 != enabledTwoPiece.end(); it2++) {
					auto key2 = *it2;
					if (key == key2) continue;
					auto &set2 = Artifact::sets.at(key2);
					ret.bonus2.emplace(Stats::ArtifactBonus{
						.setPtr = set2,
						.bonusPtr = set2.data.twoPc,
					});
					for (size_t k = 0; k < 5; k++) {
						if (k == i || k == j) continue;
						auto retSwapped = ret;
						// Swap the positions
						if (k < i) {
							retSwapped.bonus2.emplace(retSwapped.bonus1.value());
							retSwapped.bonus1.emplace(Stats::ArtifactBonus{
								.setPtr = set2,
								.bonusPtr = set2.data.twoPc,
							});
						}
						for (size_t l = k + 1; l < 5; l++) {
							if (l == i || l == j) continue;
							auto ret2 = retSwapped;
							ret2.filters.at(k).set = key2;
							ret2.filters.at(l).set = key2;

							filters.emplace_back(ret2);
						}
					}
				}
			}
		}
	}
	// Rainbow filter
	if (fiveRainbow)
		filters.emplace_back(ArtifactFilter{});

	auto end_filterGen = std::chrono::high_resolution_clock::now();
	std::println("filter gen {}", std::chrono::duration_cast<std::chrono::microseconds>(end_filterGen - start_filterGen));

	auto start_artifactDuplication = std::chrono::high_resolution_clock::now();
	std::vector<Artifact::Instance> artifacts;
	for (const auto &[_, artifact]: ::Store::artifacts) {
		artifacts.emplace_back(artifact);
	}
	auto end_artifactDuplication = std::chrono::high_resolution_clock::now();
	std::println("artifact duplication {}", std::chrono::duration_cast<std::chrono::microseconds>(end_artifactDuplication - start_artifactDuplication));

	auto prevLoadout = character.loadout.artifact.equipped;

	auto initialArtifacts = ArtifactFilter{}.filter(artifacts);
	std::array<Stats::Sheet<float>, 5> statsForSlot = getMaxStatsForSlots(initialArtifacts);
	// Roughly sort the artifact based on potential. Helps a lot when splitting
	for (size_t i = 0; i < 5; i++) {
		auto &artis = initialArtifacts.entries[i];
		for (size_t index = 0; index < 5; index++) {
			character.loadout.artifact.sheet.equippedArtifacts[index] = &statsForSlot[index];
		}
		std::sort(artis.begin(), artis.end(), [&](Artifact::Instance *art1, Artifact::Instance *art2) -> bool {
			character.loadout.artifact.sheet.equippedArtifacts[i] = &art1->stats;
			auto val1 = optimizedNode.eval(ctx);
			character.loadout.artifact.sheet.equippedArtifacts[i] = &art2->stats;
			auto val2 = optimizedNode.eval(ctx);
			return val1 < val2;
		});
	}

	Solutions solutions{};
	std::atomic<uint64_t> combed = 0;
	auto filterCount = filters.size();

	auto start = std::chrono::high_resolution_clock::now();
	std::for_each(
		std::execution::parallel_unsequenced_policy{},
		filters.begin(), filters.end(),
		[&initialArtifacts, &combed, &solutions, &optimizedNode = optimizedNode, &character_original = character, filterCount, &initialCtx = ctx](const ArtifactFilter &filter) {
			auto character = character_original;
			Team::Instance team{
				.instanceKey{},
				.stats = initialCtx.team,
			};
			for (auto &characterPtr: team.stats.characters) {
				if (characterPtr == &character_original) characterPtr = &character;
			}
			auto ctx = Formula::Context{
				.source = character.loadout,
				.active = character.loadout,
				.team = team.stats,
				.enemy = initialCtx.enemy,
				.reaction = initialCtx.reaction,
			};
			auto filtered = filter.filter(initialArtifacts);
			for (auto [slotPtr, filtered]: std::views::zip(Stats::Artifact::Slotted::getMembers(), filtered.entries)) {
				auto &slot = std::invoke(slotPtr, character.loadout.artifact.equipped);
				if (!filtered.empty()) slot = filtered.front()->key;
			}
			character.loadout.artifact.refreshStats();

			bnb(filtered, solutions, character, ctx, optimizedNode, filter.bonus1, filter.bonus2);

			combed++;
			std::println("Max dmg: {} {}/{} ({}%)", solutions.maxScore, combed.load(), filterCount, (static_cast<float>(combed) / static_cast<float>(filterCount)) * 100.f);
		}
	);

	character.loadout.artifact.equipped = prevLoadout;
	character.loadout.artifact.refreshStats();
	for (const auto &solution: solutions.solutions) {
		std::println("------------------------------------------------");
		std::println("Solution dmg: {}", solution.score);
		for (const auto &ptr: Stats::Artifact::Slotted::getMembers()) {
			auto &slot = std::invoke(ptr, solution.artifacts);
			if (!slot) continue;
			auto &artifact = ::Store::artifacts.at(slot);
			std::println(
				"{} {} {} Lvl{} ({} {}, {} {}, {} {}, {} {})", Artifact::sets.at(artifact.set).name, Utils::Stringify(artifact.slot), Utils::Stringify(artifact.mainStat), artifact.level,
				Utils::Stringify(artifact.subStats.at(0)->stat), Formula::Percentage({}, artifact.subStats.at(0)->value, Utils::isPercentage(artifact.subStats.at(0)->stat)),
				Utils::Stringify(artifact.subStats.at(1)->stat), Formula::Percentage({}, artifact.subStats.at(1)->value, Utils::isPercentage(artifact.subStats.at(1)->stat)),
				Utils::Stringify(artifact.subStats.at(2)->stat), Formula::Percentage({}, artifact.subStats.at(2)->value, Utils::isPercentage(artifact.subStats.at(2)->stat)),
				Utils::Stringify(artifact.subStats.at(3)->stat), Formula::Percentage({}, artifact.subStats.at(3)->value, Utils::isPercentage(artifact.subStats.at(3)->stat))
			);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::println("------------------------------------------------");
	std::println("Optimizing done: Best dmg {}, time taken {}", solutions.maxScore, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

	return solutions;
}
