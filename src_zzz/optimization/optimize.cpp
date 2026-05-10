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

	discs.erase(
		std::remove_if(discs.begin(), discs.end(), [&](const Disc::Instance &disc) {
			switch (disc.partition) {
				case Disc::Partition::four:
					return !options.partition4MainStats.at(disc.mainStat);
				case Disc::Partition::five:
					return !options.partition5MainStats.at(disc.mainStat);
				case Disc::Partition::six:
					return !options.partition6MainStats.at(disc.mainStat);
				default:
					return false;
			}
		}),
		discs.end()
	);

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

	std::vector<BnbState> states{};

	auto [enabledTwoPiece, enabledFourPiece] = options.makeEnabledSets();

	// 4pc filters
	for (const auto &key: enabledFourPiece) {
		BnbState state{
			.emptySlotAllowance = 0,
		};
		const auto &set = Disc::sets.at(key);
		state.sets[0] = BnbState::Set{
			.set = key,
			.filledSlotCountRequirement = 4,
		};
		state.bonuses[0] = Stats::DiscBonus{
			.setPtr = &set,
			.bonusPtr = &set.data.twoPc,
		};
		state.bonuses[1] = Stats::DiscBonus{
			.setPtr = &set,
			.bonusPtr = &set.data.fourPc,
		};

		for (const auto &key2: enabledTwoPiece) {
			if (key == key2) continue;
			const auto &set2 = Disc::sets.at(key2);
			state.sets[1] = BnbState::Set{
				.set = key2,
				.filledSlotCountRequirement = 2,
			};
			state.bonuses[2] = Stats::DiscBonus{
				.setPtr = &set2,
				.bonusPtr = &set2.data.twoPc,
			};
			state.targetHash = SlotHash(state.bonuses[0], state.bonuses[1], state.bonuses[2]);
			states.emplace_back(state);
		}
	}

	for (auto it1 = enabledTwoPiece.begin(); it1 != enabledTwoPiece.end(); it1++) {
		for (auto it2 = std::next(it1); it2 != enabledTwoPiece.end(); it2++) {
			for (auto it3 = std::next(it2); it3 != enabledTwoPiece.end(); it3++) {
				const auto &key1 = *it1;
				const auto &key2 = *it2;
				const auto &key3 = *it3;
				if (key1 == key2 || key1 == key3 || key2 == key3) continue;
				BnbState state{
					.emptySlotAllowance = 0,
				};
				const auto &set1 = Disc::sets.at(key1);
				const auto &set2 = Disc::sets.at(key2);
				const auto &set3 = Disc::sets.at(key3);
				state.sets[0] = BnbState::Set{
					.set = key1,
					.filledSlotCountRequirement = 2,
				};
				state.sets[1] = BnbState::Set{
					.set = key2,
					.filledSlotCountRequirement = 2,
				};
				state.sets[2] = BnbState::Set{
					.set = key3,
					.filledSlotCountRequirement = 2,
				};
				state.bonuses[0] = Stats::DiscBonus{
					.setPtr = &set1,
					.bonusPtr = &set1.data.twoPc,
				};
				state.bonuses[1] = Stats::DiscBonus{
					.setPtr = &set2,
					.bonusPtr = &set2.data.twoPc,
				};
				state.bonuses[2] = Stats::DiscBonus{
					.setPtr = &set3,
					.bonusPtr = &set3.data.twoPc,
				};
				state.targetHash = SlotHash(state.bonuses[0], state.bonuses[1], state.bonuses[2]);
				states.emplace_back(state);
			}
		}
	}

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
	auto stateCount = states.size();

	auto start = std::chrono::high_resolution_clock::now();
	static uint64_t runID = 0;
	runID++;

	auto preCompiledNode = optimizedNode.fold(ctx, {.enableGates = true});
	// auto preCompiledNode = optimizedNode;

	std::for_each(
		std::execution::parallel_unsequenced_policy{},
		states.begin(), states.end(),
		[&initialDiscs, &combed, &solutions, &optimizedNode = preCompiledNode, &agent_original = agent, stateCount, &initialCtx = ctx, runID = runID](BnbState &state) {
			thread_local uint64_t localRunID = runID;
			thread_local OptimizationThreadData threadData{agent_original, initialCtx};

			if (localRunID != runID) {
				threadData.~OptimizationThreadData();
				new (&threadData) OptimizationThreadData(agent_original, initialCtx);
				localRunID = runID;
			}

			auto filtered = initialDiscs.removeUnused(state);
			filtered.assignCounts(state);
			// Help harder optimizations find the best solution faster, however it may give worse solutions for slots 2-5
			// This however could help in figuring out the single best solution when a single build is requested
			// filtered.removeInferior();
			for (auto [slotPtr, filtered]: std::views::zip(Stats::Disc::Slotted::getMembers(), filtered.entries)) {
				auto &slot = std::invoke(slotPtr, threadData.agent.state.loadout().disc.getSlotted());
				if (!filtered.empty()) slot = filtered.front()->key;
			}
			threadData.agent.state.loadout().disc.refreshStats();
			state.applySets(threadData.agent.state.loadout());

			{
				Formula::enableAllocator = true;
				auto node = optimizedNode.fold(threadData.ctx, {});
				// auto node = optimizedNode;

				// threadData.agent.state.loadout().disc.getSlotted() = prevLoadout;
				// threadData.agent.state.loadout().disc.refreshStats();
				// auto evalVal = node.eval(threadData.ctx);
				// auto printVal = node.print(threadData.ctx);

				// auto node2 = optimizedNode.fold(threadData.ctx, {});
				// auto evalVal2 = node2.eval(threadData.ctx);
				// auto printVal2 = node2.print(threadData.ctx);

				bnb(filtered, solutions, threadData.agent, threadData.ctx, node, state, {});
			}
			Formula::enableAllocator = false;
			Formula::NodeAllocator::reset();

			combed++;
			std::println("Max dmg: {} {}/{} ({}%)", solutions.maxScore, combed.load(), stateCount, (static_cast<float>(combed) / static_cast<float>(stateCount)) * 100.f);
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
