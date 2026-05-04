#include "optimize.hpp"

#include "disc/sets.hpp"
#include "discFilter.hpp"
#include "store.hpp"

#include "bnb.hpp"
#include "chrono"
#include "ranges"
#include "solution.hpp"
#include <execution>

Optimization::Solutions Optimization::Optimization::optimize() const {
	auto start_discDuplication = std::chrono::high_resolution_clock::now();
	std::vector<Disc::Instance> discs;
	discs.reserve(::Store::discs.size());
	for (const auto &[_, disc]: ::Store::discs) {
		if (!options.useEquippedDiscs && disc.equippedOn() && disc.equippedOn() != agent.instanceKey)
			continue;
		discs.emplace_back(disc);
	}
	auto end_discDuplication = std::chrono::high_resolution_clock::now();
	std::println("disc duplication {}", std::chrono::duration_cast<std::chrono::microseconds>(end_discDuplication - start_discDuplication));

	auto start_filterGen = std::chrono::high_resolution_clock::now();

	std::unordered_map<uint32_t, std::array<size_t, 6>> counts{};

	for (const auto &[key, set]: Disc::sets) {
		DiscFilter pattern{};
		pattern.filters.at(0).set = key;
		pattern.filters.at(1).set = key;
		pattern.filters.at(2).set = key;
		pattern.filters.at(3).set = key;
		pattern.filters.at(4).set = key;
		pattern.filters.at(5).set = key;

		auto res = pattern.filter(discs);

		counts[key.key] = {
			res.entries.at(0).size(),
			res.entries.at(1).size(),
			res.entries.at(2).size(),
			res.entries.at(3).size(),
			res.entries.at(4).size(),
			res.entries.at(5).size(),
		};
	}

	std::vector<DiscFilter> filters{};

	auto [enabledTwoPiece, enabledFourPiece] = options.makeEnabledSets();

	// 4pc filters
	for (const auto &key: enabledFourPiece) {
		DiscFilter pattern{};
		const auto &set = Disc::sets.at(key);
		pattern.bonus1.emplace(Stats::DiscBonus{
			.setPtr = &set,
			.bonusPtr = &set.data.twoPc,
		});
		pattern.bonus2.emplace(Stats::DiscBonus{
			.setPtr = &set,
			.bonusPtr = &set.data.fourPc,
		});
		for (auto &entry: pattern.filters) {
			entry.set = key;
		}
		filters.emplace_back(pattern);

		for (size_t i = 0; i < 6; i++) {
			bool skipFilter = false;
			for (size_t j = 0; j < 6; j++) {
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
		const auto &set = Disc::sets.at(key);
		for (size_t i = 0; i < 6; i++) {
			if (counts.at(key.key).at(i) == 0) continue;
			for (size_t j = i + 1; j < 6; j++) {
				if (counts.at(key.key).at(j) == 0) continue;
				DiscFilter ret{};
				ret.bonus1.emplace(Stats::DiscBonus{
					.setPtr = &set,
					.bonusPtr = &set.data.twoPc,
				});
				ret.filters.at(i).set = key;
				ret.filters.at(j).set = key;
				if (options.threeRainbow) {
					auto retCopy = ret;
					std::vector<DiscSlotFilter *> otherSlots{};
					for (size_t k = 0; k < 6; k++) {
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
					const auto &set2 = Disc::sets.at(key2);
					ret.bonus2.emplace(Stats::DiscBonus{
						.setPtr = &set2,
						.bonusPtr = &set2.data.twoPc,
					});
					for (size_t k = 0; k < 6; k++) {
						if (k == i || k == j) continue;
						if (counts.at(key2.key).at(k) == 0) continue;
						auto retSwapped = ret;
						// Swap the positions
						if (k < i) {
							retSwapped.bonus2.emplace(retSwapped.bonus1.value());
							retSwapped.bonus1.emplace(Stats::DiscBonus{
								.setPtr = &set2,
								.bonusPtr = &set2.data.twoPc,
							});
						}
						for (size_t l = k + 1; l < 6; l++) {
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
		filters.emplace_back(DiscFilter{});

	auto end_filterGen = std::chrono::high_resolution_clock::now();
	std::println("filter gen {}", std::chrono::duration_cast<std::chrono::microseconds>(end_filterGen - start_filterGen));
	auto start_sorting = std::chrono::high_resolution_clock::now();
	auto prevLoadout = agent.state.loadout().disc.getSlotted();

	auto initialDiscs = DiscFilter{}.filter(discs);
	std::array<Stats::Sheet<float>, 6> statsForSlot = getMaxStatsForSlots(initialDiscs);
	auto compiledNode = optimizedNode.fold(ctx, {});

	auto &loadout = agent.state.loadout();
	// Roughly sort the disc based on potential. Helps a lot when splitting
	for (size_t i = 0; i < 6; i++) {
		auto &artis = initialDiscs.entries.at(i);
		for (size_t index = 0; index < 6; index++) {
			loadout.disc.sheet.equippedDiscs.at(index) = &statsForSlot.at(index);
		}
		std::sort(artis.begin(), artis.end(), [&](Disc::Instance *art1, Disc::Instance *art2) -> bool {
			loadout.disc.sheet.equippedDiscs.at(i) = &art1->stats;
			auto val1 = compiledNode.eval(ctx);
			loadout.disc.sheet.equippedDiscs.at(i) = &art2->stats;
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
	static uint64_t runID = 0;
	runID++;

	auto preCompiledNode = optimizedNode.fold(ctx, {.enableGates = true});

	std::for_each(
		std::execution::parallel_unsequenced_policy{},
		filters.begin(), filters.end(),
		[&initialDiscs, &combed, &solutions, &optimizedNode = preCompiledNode, &agent_original = agent, filterCount, &initialCtx = ctx, runID = runID](const DiscFilter &filter) {
			thread_local uint64_t localRunID = runID;
			thread_local OptimizationThreadData threadData{agent_original, initialCtx};

			if (localRunID != runID) {
				threadData.~OptimizationThreadData();
				new (&threadData) OptimizationThreadData(agent_original, initialCtx);
				localRunID = runID;
			}

			auto filtered = filter.filter(initialDiscs);
			// Help harder optimizations find the best solution faster, however it may give worse solutions for slots 2-5
			// This however could help in figuring out the single best solution when a single build is requested
			// filtered.removeInferior();
			for (auto [slotPtr, filtered]: std::views::zip(Stats::Disc::Slotted::getMembers(), filtered.entries)) {
				auto &slot = std::invoke(slotPtr, threadData.agent.state.loadout().disc.getSlotted());
				if (!filtered.empty()) slot = filtered.front()->key;
			}
			threadData.agent.state.loadout().disc.refreshStats();

			{
				Formula::enableAllocator = true;
				auto node = optimizedNode.fold(threadData.ctx, {});

				bnb(filtered, solutions, threadData.agent, threadData.ctx, node, filter.bonus1, filter.bonus2, {});
			}
			Formula::enableAllocator = false;
			Formula::NodeAllocator::reset();

			combed++;
			std::println("Max dmg: {} {}/{} ({}%)", solutions.maxScore, combed.load(), filterCount, (static_cast<float>(combed) / static_cast<float>(filterCount)) * 100.f);
		}
	);

	agent.state.loadout().disc.equipped = prevLoadout;
	agent.state.loadout().disc.refreshStats();
	// for (const auto &solution: solutions.solutions) {
	// 	std::println("------------------------------------------------");
	// 	std::println("Solution dmg: {}", solution.score);
	// 	for (const auto &ptr: Stats::Disc::Slotted::getMembers()) {
	// 		auto &slot = std::invoke(ptr, solution.discs);
	// 		if (!slot) continue;
	// 		auto &disc = ::Store::discs.at(slot);
	// 		std::println(
	// 			"{} {} {} Lvl{} ({} {}, {} {}, {} {}, {} {})", Disc::sets.at(disc.set).name, Utils::Stringify(disc.slot), Utils::Stringify(disc.mainStat), disc.level,
	// 			Utils::Stringify(disc.subStats.at(0).stat.value()), Formula::Percentage({}, disc.subStats.at(0).value, Utils::isPercentage(disc.subStats.at(0).stat.value())),
	// 			Utils::Stringify(disc.subStats.at(1).stat.value()), Formula::Percentage({}, disc.subStats.at(1).value, Utils::isPercentage(disc.subStats.at(1).stat.value())),
	// 			Utils::Stringify(disc.subStats.at(2).stat.value()), Formula::Percentage({}, disc.subStats.at(2).value, Utils::isPercentage(disc.subStats.at(2).stat.value())),
	// 			Utils::Stringify(disc.subStats.at(3).stat.value()), Formula::Percentage({}, disc.subStats.at(3).value, Utils::isPercentage(disc.subStats.at(3).stat.value()))
	// 		);
	// 	}
	// }

	auto end = std::chrono::high_resolution_clock::now();
	std::println("------------------------------------------------");
	std::println("Optimizing done: Best dmg {}, time taken {}", solutions.maxScore, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

	return solutions;
}
Optimization::OptimizationThreadData::OptimizationThreadData(Agent::Instance &agent, const Formula::Context &ctx)
	: agent(agent),
	  teamAgents{},
	  team{
		  .instanceKey{},
		  .stats = ctx.team,
	  },
	  ctx{
		  .source = this->agent.state,
		  .active = this->agent.state,
		  .team = this->team.stats,
		  .enemy = ctx.enemy,
		  .reaction = ctx.reaction,
	  } {
	for (auto [index, agentPtr]: std::views::enumerate(this->team.stats.agents)) {
		if (agentPtr == &agent) {
			agentPtr = &this->agent;
		} else if (agentPtr) {
			agentPtr = &teamAgents.at(index).emplace(*agentPtr);
		}
	}
}
