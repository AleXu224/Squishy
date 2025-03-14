#include "optimize.hpp"

#include "artifactFilter.hpp"
#include "store.hpp"

#include "chrono"
#include <execution>
#include <numeric>
#include <random>


namespace {
	struct Solution {
		Stats::Artifact::Slotted artifacts;
		float score;
	};

	struct Solutions {
		std::mutex mtx;
		std::array<Solution, 5> solutions{};
		float maxScore = 0.f;
		float minScore = 0.f;

		void addSolution(const Stats::Artifact::Slotted &artifacts, float score) {
			std::scoped_lock _{mtx};
			if (score < minScore) return;

			for (auto &solution: solutions) {
				if (solution.score < score) std::swap(solution, solutions.back());
			}
			for (auto &solution: solutions) {
				if (solution.score < score) {
					solution = Solution{
						.artifacts = artifacts,
						.score = score,
					};
					break;
				}
			}
			maxScore = solutions.front().score;
			minScore = solutions.back().score;
		}
	};

	void bnb(Optimization::FilteredArtifacts &artifacts, Solutions &solutions, Character::Instance &character, const Formula::Context &ctx, const Formula::FloatNode &node) {
		std::array<Stats::Sheet<float>, 5> statsForSlot{};

		for (auto &&[slot, artifacts]: std::views::zip(statsForSlot, artifacts.entries)) {
			for (const auto &artifact: artifacts) {
				auto &mainStatSlot = slot.fromStat(artifact.instance->mainStat);
				auto &mainStatArtifact = artifact.instance->stats.fromStat(artifact.instance->mainStat);
				mainStatSlot = std::max(mainStatSlot, mainStatArtifact);

				for (const auto &subStat: artifact.instance->subStats) {
					if (!subStat.has_value()) continue;
					auto &statSlot = slot.fromStat(subStat->stat);
					auto &statArtifact = artifact.instance->stats.fromStat(subStat->stat);
					statSlot = std::max(statSlot, statArtifact);
				}
			}
		}

		for (size_t i = 0; i < 5; i++) {
			auto &artis = artifacts.entries[i];

			artis.erase(
				std::remove_if(artis.begin(), artis.end(), [&](const auto &artifact) {
					for (size_t index = 0; index < 5; index++) {
						if (size_t(index) == i) {
							character.loadout.artifact.sheet.equippedArtifacts[index] = &artifact.instance->stats;
							continue;
						}
						character.loadout.artifact.sheet.equippedArtifacts[index] = &statsForSlot[index];
					}

					return node.eval(ctx) < solutions.minScore;
				}),
				artis.end()
			);
		}

		if (artifacts.getCombCount() <= 32) {
			for (const auto &[flower, plume, sands, goblet, circlet]: std::views::cartesian_product(artifacts.entries.at(0), artifacts.entries.at(1), artifacts.entries.at(2), artifacts.entries.at(3), artifacts.entries.at(4))) {
				character.loadout.artifact.equipped.flower = flower.instance->key;
				character.loadout.artifact.equipped.plume = plume.instance->key;
				character.loadout.artifact.equipped.sands = sands.instance->key;
				character.loadout.artifact.equipped.goblet = goblet.instance->key;
				character.loadout.artifact.equipped.circlet = circlet.instance->key;
				character.loadout.artifact.refreshStats();
				auto dmg = node.eval(ctx);
				solutions.addSolution(character.loadout.artifact.equipped, dmg);
			}
			return;
		}

		size_t biggestSlot = 0;
		size_t biggestSlotIndex = 0;
		for (size_t i = 0; i < 5; i++) {
			auto size = artifacts.entries[i].size();
			if (size > biggestSlot) {
				biggestSlot = size;
				biggestSlotIndex = i;
			}
		}

		auto &chosenSplitSlot = artifacts.entries[biggestSlotIndex];
		auto midPoint = std::midpoint(size_t(0), chosenSplitSlot.size());
		auto copy1 = artifacts;
		auto copy2 = artifacts;
		copy1.entries[biggestSlotIndex] = std::vector(artifacts.entries[biggestSlotIndex].begin(), artifacts.entries[biggestSlotIndex].begin() + midPoint);
		copy2.entries[biggestSlotIndex] = std::vector(artifacts.entries[biggestSlotIndex].begin() + midPoint, artifacts.entries[biggestSlotIndex].end());

		bnb(copy1, solutions, character, ctx, node);
		bnb(copy2, solutions, character, ctx, node);
	}
}// namespace

void Optimization::Optimization::optimize() const {
	auto start_filterGen = std::chrono::high_resolution_clock::now();

	std::vector<ArtifactFilter> filters{};

	// 4pc filters
	for (const auto &[key, _]: Artifact::sets) {
		ArtifactFilter pattern{};
		for (auto &entry: pattern.filters) {
			entry.set = key;
		}

		for (size_t i = 0; i < 5; i++) {
			auto ret = pattern;
			ret.filters.at(i).set = std::nullopt;
			filters.emplace_back(ret);
		}
	}

	// 2pc filters
	for (auto it = Artifact::sets.begin(); it != Artifact::sets.end(); it++) {
		auto key = it->first;
		for (size_t i = 0; i < 5; i++) {
			for (size_t j = i + 1; j < 5; j++) {
				ArtifactFilter ret{};
				ret.filters.at(i).set = key;
				ret.filters.at(j).set = key;
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

				// 2pc2pc
				for (auto it2 = std::next(it); it2 != Artifact::sets.end(); it2++) {
					auto key2 = it2->first;
					if (key == key2) continue;

					for (size_t k = 0; k < 5; k++) {
						if (k == i || k == j) continue;
						for (size_t l = k + 1; l < 5; l++) {
							if (l == i || l == j) continue;
							auto ret2 = ret;
							ret2.filters.at(k).set = key2;
							ret2.filters.at(l).set = key2;

							filters.emplace_back(ret2);
						}
					}
				}
			}
		}
	}
	auto end_filterGen = std::chrono::high_resolution_clock::now();
	std::println("filter gen {}", std::chrono::duration_cast<std::chrono::microseconds>(end_filterGen - start_filterGen));

	auto start_artifactDuplication = std::chrono::high_resolution_clock::now();
	std::vector<Artifact::Instance> artifacts;
	for (const auto &[_, artifact]: ::Store::artifacts) {
		artifacts.emplace_back(artifact);
	}
	auto end_artifactDuplication = std::chrono::high_resolution_clock::now();
	std::println("artifact duplication {}", std::chrono::duration_cast<std::chrono::microseconds>(end_artifactDuplication - start_artifactDuplication));

	auto rd = std::random_device{};
	auto gen = std::mt19937(rd());

	auto prevLoadout = character.loadout.artifact.equipped;

	Solutions solutions{};
	std::atomic<uint64_t> combed = 0;
	auto filterCount = filters.size();

	auto start = std::chrono::high_resolution_clock::now();
	std::for_each(
		std::execution::parallel_unsequenced_policy{},
		filters.begin(), filters.end(),
		[&artifacts, &combed, &solutions, &optimizedNode = optimizedNode, character = character, gen, filterCount, &initialCtx = ctx](const auto &filter) mutable {
			auto ctx = Formula::Context{
				.source = character.loadout,
				.active = character.loadout,
				.team = initialCtx.team,
				.enemy = initialCtx.enemy,
				.reaction = initialCtx.reaction,
			};
			auto filtered = filter.filter(artifacts);
			for (auto [slotPtr, filtered]: std::views::zip(Stats::Artifact::Slotted::getMembers(), filtered.entries)) {
				auto &slot = std::invoke(slotPtr, character.loadout.artifact.equipped);
				std::uniform_int_distribution dist{0ull, filtered.size() - 1};
				if (!filtered.empty()) slot = (filtered.begin() + dist(gen))->instance->key;
			}
			character.loadout.artifact.refreshStats();

			bnb(filtered, solutions, character, ctx, optimizedNode);

			combed++;
			std::println("Max dmg: {} {}/{} ({}%)", solutions.maxScore, combed.load(), filterCount, (static_cast<float>(combed) / static_cast<float>(filterCount)) * 100.f);
		}
	);

	character.loadout.artifact.equipped = prevLoadout;
	character.loadout.artifact.refreshStats();
	for (const auto &solution: solutions.solutions) {
		std::println("Solution dmg: {}", solution.score);
		for (const auto &ptr: Stats::Artifact::Slotted::getMembers()) {
			auto &slot = std::invoke(ptr, solution.artifacts);
			auto &artifact = ::Store::artifacts.at(slot);
			std::println(
				"{} {} {} {} {} ({} {} {} {})", artifact.key.key, Artifact::sets.at(artifact.set).name, Utils::Stringify(artifact.slot), Utils::Stringify(artifact.mainStat), artifact.level,
				Utils::Stringify(artifact.subStats.at(0)->stat),
				Utils::Stringify(artifact.subStats.at(1)->stat),
				Utils::Stringify(artifact.subStats.at(2)->stat),
				Utils::Stringify(artifact.subStats.at(3)->stat)
			);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::println("{} filters, {}", filters.size(), std::chrono::duration_cast<std::chrono::microseconds>(end - start));
}
