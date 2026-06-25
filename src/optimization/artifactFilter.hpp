#pragma once

#include "artifact/instance.hpp"
#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "ranges"
#include "stats/artifact.hpp"
#include <algorithm>
#include <optional>


namespace Optimization {
	uint64_t getHash(Stat mainStat, std::optional<Stat> sub1, std::optional<Stat> sub2, std::optional<Stat> sub3, std::optional<Stat> sub4) {
		uint64_t mainStatVal = static_cast<uint8_t>(mainStat);
		uint8_t sub1Val = sub1.transform([](Stat &val) {
								  return static_cast<uint8_t>(val);
							  })
							  .value_or(std::numeric_limits<uint8_t>::max());
		uint8_t sub2Val = sub2.transform([](Stat &val) {
								  return static_cast<uint8_t>(val);
							  })
							  .value_or(std::numeric_limits<uint8_t>::max());
		uint8_t sub3Val = sub3.transform([](Stat &val) {
								  return static_cast<uint8_t>(val);
							  })
							  .value_or(std::numeric_limits<uint8_t>::max());
		uint8_t sub4Val = sub4.transform([](Stat &val) {
								  return static_cast<uint8_t>(val);
							  })
							  .value_or(std::numeric_limits<uint8_t>::max());

		std::array<uint8_t, 4> vals{sub1Val, sub2Val, sub3Val, sub4Val};
		std::sort(vals.begin(), vals.end());

		mainStatVal = mainStatVal << 32;
		mainStatVal += vals[0] << 24;
		mainStatVal += vals[1] << 16;
		mainStatVal += vals[2] << 8;
		mainStatVal += vals[3];

		return mainStatVal;
	}

	struct ArtifactEntryStats {
		struct Substat {
			std::optional<Stat> stat = std::nullopt;
			float value = 0.f;
		};

		uint8_t level;
		std::array<Substat, 4> subStats{};

		Substat &getSubstat(Stat stat) {
			for (auto &substat: subStats) {
				if (!substat.stat.has_value()) {
					substat.stat = stat;
					return substat;
				}
				if (substat.stat.value() == stat) return substat;
			}
			std::unreachable();
		}

		bool operator<(const Artifact::Instance &artifact) {
			if (artifact.level > level) return false;
			for (const auto &subStat: artifact.subStats) {
				if (!subStat.stat.has_value()) continue;
				if (getSubstat(subStat.stat.value()).value > subStat.value) return false;
			}
			return true;
		}

		void assign(const Artifact::Instance &artifact) {
			level = artifact.level;
			for (const auto &subStat: artifact.subStats) {
				if (!subStat.stat.has_value()) continue;
				getSubstat(subStat.stat.value()).value = subStat.value;
			}
		}
	};

	struct FilteredArtifacts {
		std::array<std::vector<Artifact::Instance *>, 5> entries{};

		size_t getCombCount() const {
			return entries[0].size()
				 * entries[1].size()
				 * entries[2].size()
				 * entries[3].size()
				 * entries[4].size();
		}

		void removeInferior() {
			for (size_t i = 0; i < 5; i++) {
				std::unordered_map<uint64_t, ArtifactEntryStats> entryStats;
				auto &entryVec = entries[i];
				for (const auto &entry: entryVec) {
					auto &entryStat = entryStats[getHash(entry->mainStat, entry->subStats[0].stat, entry->subStats[1].stat, entry->subStats[2].stat, entry->subStats[3].stat)];
					if (entryStat < *entry) {
						entryStat.assign(*entry);
					}
				}
				entryVec.erase(
					std::remove_if(entryVec.begin(), entryVec.end(), [&](Artifact::Instance *entry) {
						auto &entryStat = entryStats[getHash(entry->mainStat, entry->subStats[0].stat, entry->subStats[1].stat, entry->subStats[2].stat, entry->subStats[3].stat)];
						return entryStat < *entry;
					}),
					entryVec.end()
				);
			}
		}
	};

	struct ArtifactSlotFilter {
		Artifact::Slot slot;
		std::optional<Artifact::SetKey> set{};
		std::optional<Artifact::SetKey> notSet{};

		[[nodiscard]] bool eval(const Artifact::Instance &instance) const {
			if (instance.slot != slot) return false;
			if (set && instance.set != set) return false;
			if (notSet && instance.set == notSet) return false;

			return true;
		}
	};

	struct ArtifactFilter {
		std::array<ArtifactSlotFilter, 5> filters{
			ArtifactSlotFilter{.slot = Artifact::Slot::flower},
			ArtifactSlotFilter{.slot = Artifact::Slot::plume},
			ArtifactSlotFilter{.slot = Artifact::Slot::sands},
			ArtifactSlotFilter{.slot = Artifact::Slot::goblet},
			ArtifactSlotFilter{.slot = Artifact::Slot::circlet},
		};
		std::optional<Stats::ArtifactBonus> bonus1;
		std::optional<Stats::ArtifactBonus> bonus2;

		FilteredArtifacts filter(std::vector<Artifact::Instance> &artifacts) const {
			FilteredArtifacts ret{};
			for (auto [dst, filter]: std::views::zip(ret.entries, filters)) {
				for (auto &artifact: artifacts) {
					if (filter.eval(artifact)) dst.emplace_back(&artifact);
				}
			}

			return ret;
		}
		FilteredArtifacts filter(FilteredArtifacts &artifacts) const {
			FilteredArtifacts ret{};
			for (auto [artifacts, dst, filter]: std::views::zip(artifacts.entries, ret.entries, filters)) {
				for (auto &artifact: artifacts) {
					if (filter.eval(*artifact)) dst.emplace_back(artifact);
				}
			}

			return ret;
		}
	};

	// Combines all the artifacts of a slot into one unrealistically optimistic one to get the max potential
	static inline std::array<Stats::Sheet<float>, 5> getMaxStatsForSlots(auto &&artifacts) {
		std::array<Stats::Sheet<float>, 5> statsForSlot{};

		for (auto &&[slot, artifacts]: std::views::zip(statsForSlot, artifacts.entries)) {
			for (const auto &artifact: artifacts) {
				auto &mainStatSlot = slot.fromStat(artifact->mainStat);
				auto &mainStatArtifact = artifact->stats.fromStat(artifact->mainStat);
				mainStatSlot = std::max(mainStatSlot, mainStatArtifact);

				for (const auto &subStat: artifact->subStats) {
					if (!subStat.stat.has_value()) continue;
					auto &statSlot = slot.fromStat(subStat.stat.value());
					auto &statArtifact = artifact->stats.fromStat(subStat.stat.value());
					statSlot = std::max(statSlot, statArtifact);
				}
			}
		}
		return statsForSlot;
	}

	struct MaxStatsByMainstat {
		struct SlotStatsWrapper {
			Stats::Sheet<float> stats{};
			Stat mainStat{};
			bool foundUpgrade = false;
		};
		using SlotStats = std::vector<SlotStatsWrapper>;

		SlotStats slot1{};
		SlotStats slot2{};
		SlotStats slot3{};
		SlotStats slot4{};
		SlotStats slot5{};

		void clear() {
			slot1.clear();
			slot2.clear();
			slot3.clear();
			slot4.clear();
			slot5.clear();
		}

		size_t getCombCount() const {
			return slot1.size()
				 * slot2.size()
				 * slot3.size()
				 * slot4.size()
				 * slot5.size();
		}

		SlotStatsWrapper &getSlotStatsForMainStat(Artifact::Slot slot, Stat mainStat) {
			auto &slotStats = *getSlots().at(static_cast<size_t>(slot));
			for (auto &slotStat: slotStats) {
				if (slotStat.mainStat == mainStat) return slotStat;
			}
			slotStats.emplace_back(SlotStatsWrapper{.mainStat = mainStat});
			return slotStats.back();
		}

		std::array<SlotStats *, 5> getSlots() {
			return {&slot1, &slot2, &slot3, &slot4, &slot5};
		}
	};

	static inline MaxStatsByMainstat &getMaxStatsForSlotsByMainstat(const FilteredArtifacts &artifacts) {
		thread_local MaxStatsByMainstat statsForSlot{};

		statsForSlot.clear();
		for (auto &&[slotMap, artifacts]: std::views::zip(statsForSlot.getSlots(), artifacts.entries)) {
			for (const auto &artifact: artifacts) {
				auto &slot = statsForSlot.getSlotStatsForMainStat(artifact->slot, artifact->mainStat);
				auto &mainStatSlot = slot.stats.fromStat(artifact->mainStat);
				auto &mainStatArtifact = artifact->stats.fromStat(artifact->mainStat);
				mainStatSlot = std::max(mainStatSlot, mainStatArtifact);

				for (const auto &subStat: artifact->subStats) {
					if (!subStat.stat.has_value()) continue;
					auto &statSlot = slot.stats.fromStat(subStat.stat.value());
					auto &statArtifact = artifact->stats.fromStat(subStat.stat.value());
					statSlot = std::max(statSlot, statArtifact);
				}
			}
		}
		return statsForSlot;
	}
}// namespace Optimization