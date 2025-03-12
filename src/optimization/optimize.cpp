#include "optimize.hpp"

#include "artifactFilter.hpp"
#include "store.hpp"

#include "chrono"
#include <random>

void Optimization::Optimization::optimize() const {
	auto start_filterGen = std::chrono::high_resolution_clock::now();

	std::vector<ArtifactFilter> filters{};

	// 2pc filters
	for (auto it = Artifact::sets.begin(); it != Artifact::sets.end(); it++) {
		auto key = it->first;
		for (size_t i = 0; i < 5; i++) {
			for (size_t j = i + 1; j < 5; j++) {
				ArtifactFilter ret{};
				ret.filters.at(i).set = key;
				ret.filters.at(j).set = key;
				filters.emplace_back(ret);

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

	float bestDmg = 0.f;
	Stats::Artifact::Slotted bestLoadout;

	auto start = std::chrono::high_resolution_clock::now();
	for (const auto &filter: filters) {
		auto filtered = filter.filter(artifacts);
		for (auto [slotPtr, filtered]: std::views::zip(Stats::Artifact::Slotted::getMembers(), filtered.entries)) {
			auto &slot = std::invoke(slotPtr, character.loadout.artifact.equipped);
			std::uniform_int_distribution dist{0ull, filtered.size() - 1};
			if (!filtered.empty()) slot = (filtered.begin() + dist(gen))->instance->key;
		}
		character.loadout.artifact.refreshStats();

		bool modified = true;
		while (modified) {
			modified = false;

			for (auto [slotPtr, artifacts]: std::views::zip(Stats::Artifact::Slotted::getMembers(), filtered.entries)) {
				auto &slot = std::invoke(slotPtr, character.loadout.artifact.equipped);
				for (auto &artifact: artifacts) {
					slot = artifact.instance->key;
					character.loadout.artifact.refreshStats();
					auto dmg = optimizedNode.eval(ctx);
					artifact.maxScore = std::max(artifact.maxScore, dmg);
					if (dmg > bestDmg) {
						bestDmg = dmg;
						bestLoadout = character.loadout.artifact.equipped;
						modified = true;
					}
				}
				slot = std::invoke(slotPtr, bestLoadout);
			}
		}
		auto topFiltered = filtered.getTop();

		for (const auto &[flower, plume, sands, goblet, circlet]: std::views::cartesian_product(topFiltered.entries.at(0), topFiltered.entries.at(1), topFiltered.entries.at(2), topFiltered.entries.at(3), topFiltered.entries.at(4))) {
			character.loadout.artifact.equipped.flower = flower.instance->key;
			character.loadout.artifact.equipped.plume = plume.instance->key;
			character.loadout.artifact.equipped.sands = sands.instance->key;
			character.loadout.artifact.equipped.goblet = goblet.instance->key;
			character.loadout.artifact.equipped.circlet = circlet.instance->key;
			character.loadout.artifact.refreshStats();
			auto dmg = optimizedNode.eval(ctx);
			if (dmg > bestDmg) {
				bestDmg = dmg;
				bestLoadout = character.loadout.artifact.equipped;
			}
		}
	}

	character.loadout.artifact.equipped = prevLoadout;
	character.loadout.artifact.refreshStats();
	std::println("Best dmg: {}", bestDmg);
	for (const auto &ptr: Stats::Artifact::Slotted::getMembers()) {
		auto &slot = std::invoke(ptr, bestLoadout);
		auto &artifact = ::Store::artifacts.at(slot.value());
		std::println(
			"{} {} {} {} {} ({} {} {} {})", artifact.key.key, Artifact::sets.at(artifact.set).name, Utils::Stringify(artifact.slot), Utils::Stringify(artifact.mainStat), artifact.level,
			Utils::Stringify(artifact.subStats.at(0)->stat),
			Utils::Stringify(artifact.subStats.at(1)->stat),
			Utils::Stringify(artifact.subStats.at(2)->stat),
			Utils::Stringify(artifact.subStats.at(3)->stat)
		);
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::println("{} filters, {}", filters.size(), std::chrono::duration_cast<std::chrono::microseconds>(end - start));
}
