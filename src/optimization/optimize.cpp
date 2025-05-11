#include "optimize.hpp"

#include "artifact/sets.hpp"
#include "artifactFilter.hpp"
#include "store.hpp"

#include "bnb.hpp"
#include "chrono"
#include "ranges"
#include "solution.hpp"
#include <execution>

Optimization::Solutions Optimization::Optimization::optimize() const {
	auto start_artifactDuplication = std::chrono::high_resolution_clock::now();
	std::vector<Artifact::Instance> artifacts;
	artifacts.reserve(::Store::artifacts.size());
	for (const auto &[_, artifact]: ::Store::artifacts) {
		artifacts.emplace_back(artifact);
	}
	auto end_artifactDuplication = std::chrono::high_resolution_clock::now();
	std::println("artifact duplication {}", std::chrono::duration_cast<std::chrono::microseconds>(end_artifactDuplication - start_artifactDuplication));

	auto start_filterGen = std::chrono::high_resolution_clock::now();

	std::unordered_map<uint32_t, std::array<size_t, 5>> counts{};

	for (const auto &[key, set]: Artifact::sets) {
		ArtifactFilter pattern{};
		pattern.filters.at(0).set = key;
		pattern.filters.at(1).set = key;
		pattern.filters.at(2).set = key;
		pattern.filters.at(3).set = key;
		pattern.filters.at(4).set = key;

		auto res = pattern.filter(artifacts);

		counts[key.key] = {
			res.entries.at(0).size(),
			res.entries.at(1).size(),
			res.entries.at(2).size(),
			res.entries.at(3).size(),
			res.entries.at(4).size(),
		};
	}

	std::vector<ArtifactFilter> filters{};

	auto [enabledTwoPiece, enabledFourPiece] = options.makeEnabledSets();

	// 4pc filters
	for (const auto &key: enabledFourPiece) {
		ArtifactFilter pattern{};
		const auto &set = Artifact::sets.at(key);
		pattern.bonus1.emplace(Stats::ArtifactBonus{
			.setPtr = &set,
			.bonusPtr = &set.data.twoPc,
		});
		pattern.bonus2.emplace(Stats::ArtifactBonus{
			.setPtr = &set,
			.bonusPtr = &set.data.fourPc,
		});
		for (auto &entry: pattern.filters) {
			entry.set = key;
		}
		filters.emplace_back(pattern);

		for (size_t i = 0; i < 5; i++) {
			bool skipFilter = false;
			for (size_t j = 0; j < 5; j++) {
				if (j == i) continue;
				if (counts.at(key.key).at(j) == 0) {
					skipFilter = true;
					break;
				}
			}
			if (skipFilter) continue;
			auto ret = pattern;
			ret.filters.at(i).set = std::nullopt;
			ret.filters.at(i).notSet = key;
			filters.emplace_back(ret);
		}
	}

	// 2pc filters
	for (auto it = enabledTwoPiece.begin(); it != enabledTwoPiece.end(); it++) {
		auto key = *it;
		const auto &set = Artifact::sets.at(key);
		for (size_t i = 0; i < 5; i++) {
			if (counts.at(key.key).at(i) == 0) continue;
			for (size_t j = i + 1; j < 5; j++) {
				if (counts.at(key.key).at(j) == 0) continue;
				ArtifactFilter ret{};
				ret.bonus1.emplace(Stats::ArtifactBonus{
					.setPtr = &set,
					.bonusPtr = &set.data.twoPc,
				});
				ret.filters.at(i).set = key;
				ret.filters.at(j).set = key;
				if (options.threeRainbow) {
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
					const auto &set2 = Artifact::sets.at(key2);
					ret.bonus2.emplace(Stats::ArtifactBonus{
						.setPtr = &set2,
						.bonusPtr = &set2.data.twoPc,
					});
					for (size_t k = 0; k < 5; k++) {
						if (k == i || k == j) continue;
						if (counts.at(key2.key).at(k) == 0) continue;
						auto retSwapped = ret;
						// Swap the positions
						if (k < i) {
							retSwapped.bonus2.emplace(retSwapped.bonus1.value());
							retSwapped.bonus1.emplace(Stats::ArtifactBonus{
								.setPtr = &set2,
								.bonusPtr = &set2.data.twoPc,
							});
						}
						for (size_t l = k + 1; l < 5; l++) {
							if (l == i || l == j) continue;
							if (counts.at(key2.key).at(l) == 0) continue;
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
	if (options.fiveRainbow)
		filters.emplace_back(ArtifactFilter{});

	auto end_filterGen = std::chrono::high_resolution_clock::now();
	std::println("filter gen {}", std::chrono::duration_cast<std::chrono::microseconds>(end_filterGen - start_filterGen));
	auto start_sorting = std::chrono::high_resolution_clock::now();
	auto prevLoadout = character.state.loadout().artifact.getSlotted();

	auto initialArtifacts = ArtifactFilter{}.filter(artifacts);
	std::array<Stats::Sheet<float>, 5> statsForSlot = getMaxStatsForSlots(initialArtifacts);
	auto compiledNode = optimizedNode.compile(ctx);

	auto &loadout = character.state.loadout();
	// Roughly sort the artifact based on potential. Helps a lot when splitting
	for (size_t i = 0; i < 5; i++) {
		auto &artis = initialArtifacts.entries.at(i);
		for (size_t index = 0; index < 5; index++) {
			loadout.artifact.sheet.equippedArtifacts.at(index) = &statsForSlot.at(index);
		}
		std::sort(artis.begin(), artis.end(), [&](Artifact::Instance *art1, Artifact::Instance *art2) -> bool {
			loadout.artifact.sheet.equippedArtifacts.at(i) = &art1->stats;
			auto val1 = compiledNode.eval(ctx);
			loadout.artifact.sheet.equippedArtifacts.at(i) = &art2->stats;
			auto val2 = compiledNode.eval(ctx);
			return val1 < val2;
		});
	}
	auto end_sorting = std::chrono::high_resolution_clock::now();
	std::println("sorting gen {}", std::chrono::duration_cast<std::chrono::microseconds>(end_sorting - start_sorting));

	Solutions solutions{};
	std::atomic<uint64_t> combed = 0;
	auto filterCount = filters.size();

	auto start = std::chrono::high_resolution_clock::now();
	std::for_each(
		std::execution::parallel_unsequenced_policy{},
		filters.begin(), filters.end(),
		[&initialArtifacts, &combed, &solutions, &optimizedNode = optimizedNode, &character_original = character, filterCount, &initialCtx = ctx](const ArtifactFilter &filter) {
			auto character = character_original;
			std::array<std::optional<Character::Instance>, 4> teamCharacters;
			Team::Instance team{
				.instanceKey{},
				.stats = initialCtx.team,
			};
			for (auto [index, characterPtr]: std::views::enumerate(team.stats.characters)) {
				if (characterPtr == &character_original) {
					characterPtr = &character;
				} else if (characterPtr) {
					characterPtr = &teamCharacters.at(index).emplace(*characterPtr);
				}
			}
			std::vector<Combo::Option> optionStore;
			auto ctx = Formula::Context{
				.source = character.state,
				.active = character.state,
				.team = team.stats,
				.enemy = initialCtx.enemy,
				.reaction = initialCtx.reaction,
				.optionStore = &optionStore,
			};
			auto filtered = filter.filter(initialArtifacts);
			// Help harder optimizations find the best solution faster, however it may give worse solutions for slots 2-5
			// This however could help in figuring out the single best solution when a single build is requested
			// filtered.removeInferior();
			for (auto [slotPtr, filtered]: std::views::zip(Stats::Artifact::Slotted::getMembers(), filtered.entries)) {
				auto &slot = std::invoke(slotPtr, character.state.loadout().artifact.getSlotted());
				if (!filtered.empty()) slot = filtered.front()->key;
			}
			character.state.loadout().artifact.refreshStats();
			auto node = optimizedNode.compile(ctx);

			bnb(filtered, solutions, character, ctx, node, filter.bonus1, filter.bonus2, {});

			combed++;
			std::println("Max dmg: {} {}/{} ({}%)", solutions.maxScore, combed.load(), filterCount, (static_cast<float>(combed) / static_cast<float>(filterCount)) * 100.f);
		}
	);

	character.state.loadout().artifact.equipped = prevLoadout;
	character.state.loadout().artifact.refreshStats();
	// for (const auto &solution: solutions.solutions) {
	// 	std::println("------------------------------------------------");
	// 	std::println("Solution dmg: {}", solution.score);
	// 	for (const auto &ptr: Stats::Artifact::Slotted::getMembers()) {
	// 		auto &slot = std::invoke(ptr, solution.artifacts);
	// 		if (!slot) continue;
	// 		auto &artifact = ::Store::artifacts.at(slot);
	// 		std::println(
	// 			"{} {} {} Lvl{} ({} {}, {} {}, {} {}, {} {})", Artifact::sets.at(artifact.set).name, Utils::Stringify(artifact.slot), Utils::Stringify(artifact.mainStat), artifact.level,
	// 			Utils::Stringify(artifact.subStats.at(0).stat.value()), Formula::Percentage({}, artifact.subStats.at(0).value, Utils::isPercentage(artifact.subStats.at(0).stat.value())),
	// 			Utils::Stringify(artifact.subStats.at(1).stat.value()), Formula::Percentage({}, artifact.subStats.at(1).value, Utils::isPercentage(artifact.subStats.at(1).stat.value())),
	// 			Utils::Stringify(artifact.subStats.at(2).stat.value()), Formula::Percentage({}, artifact.subStats.at(2).value, Utils::isPercentage(artifact.subStats.at(2).stat.value())),
	// 			Utils::Stringify(artifact.subStats.at(3).stat.value()), Formula::Percentage({}, artifact.subStats.at(3).value, Utils::isPercentage(artifact.subStats.at(3).stat.value()))
	// 		);
	// 	}
	// }

	auto end = std::chrono::high_resolution_clock::now();
	std::println("------------------------------------------------");
	std::println("Optimizing done: Best dmg {}, time taken {}", solutions.maxScore, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

	return solutions;
}
