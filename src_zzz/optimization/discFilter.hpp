#pragma once

#include "disc/instance.hpp"
#include "disc/key.hpp"
#include "disc/slot.hpp"
#include "ranges"
#include "stats/disc.hpp"
#include "stats/loadout.hpp"
#include <algorithm>
#include <optional>


namespace Optimization {
	struct SlotHash {
		uint64_t hash;

		SlotHash() : hash(0) {}

		SlotHash(const Disc::SetKey &key1, const Disc::SetKey &key2, const Disc::SetKey &key3) {
			uint32_t min_val = std::min({key1.key, key2.key, key3.key});
			uint32_t max_val = std::max({key1.key, key2.key, key3.key});

			uint32_t mid_val = (key1.key + key2.key + key3.key) - min_val - max_val;

			hash = (static_cast<uint64_t>(max_val) << 42) | (static_cast<uint64_t>(mid_val) << 21) | static_cast<uint64_t>(min_val);
		}
		SlotHash(const std::optional<Stats::DiscBonus> &key1, const std::optional<Stats::DiscBonus> &key2, const std::optional<Stats::DiscBonus> &key3) {
			uint32_t key1Val = key1.has_value() ? key1.value().setPtr->key.key : 0;
			uint32_t key2Val = key2.has_value() ? key2.value().setPtr->key.key : 0;
			uint32_t key3Val = key3.has_value() ? key3.value().setPtr->key.key : 0;

			uint32_t min_val = std::min({key1Val, key2Val, key3Val});
			uint32_t max_val = std::max({key1Val, key2Val, key3Val});

			uint32_t mid_val = (key1Val + key2Val + key3Val) - min_val - max_val;

			hash = (static_cast<uint64_t>(max_val) << 42) | (static_cast<uint64_t>(mid_val) << 21) | static_cast<uint64_t>(min_val);
		}

		auto operator<=>(const SlotHash &) const = default;
	};

	struct BnbState {
		struct Set {
			Disc::SetKey set;
			uint8_t filledSlotCountRequirement;
			std::array<uint32_t, 6> countsForSlot{};

			[[nodiscard]] uint8_t getSlotCount() const {
				uint8_t count = 0;
				for (const auto &countForSlot: countsForSlot) {
					if (countForSlot > 0) count++;
				}
				return count;
			}

			[[nodiscard]] bool isValid() const {
				if (!set) return true;
				return getSlotCount() >= filledSlotCountRequirement;
			}
		};

		std::array<std::optional<Stats::DiscBonus>, 3> bonuses{};

		uint8_t emptySlotAllowance;
		SlotHash targetHash;
		std::array<Set, 3> sets{};

		[[nodiscard]] bool isValid() const {
			for (const auto &set: sets) {
				if (!set.isValid()) return false;
			}
			std::array<uint32_t, 6> totalCountsForSlot{};
			for (uint8_t i = 0; i < 6; i++) {
				totalCountsForSlot[i] = sets[0].countsForSlot[i]
									  + sets[1].countsForSlot[i]
									  + sets[2].countsForSlot[i];
			}
			uint8_t totalEmptyCount = 6;
			for (const auto &countForSlot: totalCountsForSlot) {
				if (countForSlot > 0) totalEmptyCount--;
			}
			if (totalEmptyCount > emptySlotAllowance) return false;
			return true;
		}

		void applySets(Stats::Loadout &loadout) const {
			uint32_t index = 0;
			constexpr std::array bonusSlots{&Stats::Disc::bonus1, &Stats::Disc::bonus2, &Stats::Disc::bonus3};
			for (const auto &bonus: bonuses) {
				loadout.disc.*bonusSlots[index++] = bonus;
			}
		}

		std::array<uint32_t, 3> countSetsForDiscs(const std::vector<Disc::Instance *> &discs) const {
			std::array<uint32_t, 3> counts{};
			for (const auto &disc: discs) {
				for (size_t i = 0; i < 3; i++) {
					if (sets[i].set && disc->set == sets[i].set) {
						counts[i]++;
						break;
					}
				}
			}
			return counts;
		}

		std::array<uint32_t, 3> getCountsForIndex(size_t index) const {
			return std::array<uint32_t, 3>{sets[0].countsForSlot[index], sets[1].countsForSlot[index], sets[2].countsForSlot[index]};
		}

		void setCountsForIndex(size_t index, const std::array<uint32_t, 3> &counts) {
			for (size_t i = 0; i < 3; i++) {
				sets[i].countsForSlot[index] = counts[i];
			}
		}

		void subtractCountForIndex(size_t index, Disc::SetKey set) {
			for (size_t i = 0; i < 3; i++) {
				if (sets[i].set == set) {
					if (sets[i].countsForSlot[index] > 0) {
						sets[i].countsForSlot[index]--;
					}
					return;
				}
			}
		}
	};

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

	struct DiscEntryStats {
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

		bool operator<(const Disc::Instance &disc) {
			if (disc.level > level) return false;
			for (const auto &subStat: disc.subStats) {
				if (!subStat.stat.has_value()) continue;
				if (getSubstat(subStat.stat.value()).value > subStat.value) return false;
			}
			return true;
		}

		void assign(const Disc::Instance &disc) {
			level = disc.level;
			for (const auto &subStat: disc.subStats) {
				if (!subStat.stat.has_value()) continue;
				getSubstat(subStat.stat.value()).value = subStat.value;
			}
		}
	};

	struct FilteredDiscs {
		std::array<std::vector<Disc::Instance *>, 6> entries{};

		size_t getCombCount() const {
			return entries[0].size()
				 * entries[1].size()
				 * entries[2].size()
				 * entries[3].size()
				 * entries[4].size()
				 * entries[5].size();
		}

		FilteredDiscs removeUnused(const BnbState &state) const {
			// If some kind of rainbow build is required then this is not gonna be able to do anything so just return the same filter
			// Only required to check the last one since they are filled in order
			if (!state.bonuses[2].has_value()) {
				return *this;
			}
			std::array<Disc::SetKey, 3> requiredSets{};
			for (size_t i = 0; i < 3; i++) {
				requiredSets[i] = state.bonuses[i]->setPtr->key;
			}
			FilteredDiscs ret = *this;
			for (size_t i = 0; i < 6; i++) {
				auto &entryVec = ret.entries[i];
				entryVec.erase(
					std::remove_if(entryVec.begin(), entryVec.end(), [&](Disc::Instance *entry) {
						for (const auto &requiredSet: requiredSets) {
							if (entry->set == requiredSet) return false;
						}
						return true;
					}),
					entryVec.end()
				);
			}
			return ret;
		}

		void assignCounts(BnbState &state) const {
			for (size_t i = 0; i < 6; i++) {
				state.setCountsForIndex(i, state.countSetsForDiscs(entries[i]));
			}
		}

		void removeInferior() {
			for (size_t i = 0; i < 6; i++) {
				std::unordered_map<uint64_t, DiscEntryStats> entryStats;
				auto &entryVec = entries[i];
				for (const auto &entry: entryVec) {
					auto &entryStat = entryStats[getHash(entry->mainStat, entry->subStats[0].stat, entry->subStats[1].stat, entry->subStats[2].stat, entry->subStats[3].stat)];
					if (entryStat < *entry) {
						entryStat.assign(*entry);
					}
				}
				entryVec.erase(
					std::remove_if(entryVec.begin(), entryVec.end(), [&](Disc::Instance *entry) {
						auto &entryStat = entryStats[getHash(entry->mainStat, entry->subStats[0].stat, entry->subStats[1].stat, entry->subStats[2].stat, entry->subStats[3].stat)];
						return entryStat < *entry;
					}),
					entryVec.end()
				);
			}
		}
	};

	struct DiscSlotFilter {
		Disc::Partition partition;
		std::optional<Disc::SetKey> set{};
		std::optional<Disc::SetKey> notSet{};

		[[nodiscard]] bool eval(const Disc::Instance &instance) const {
			if (instance.partition != partition) return false;
			if (set && instance.set != set) return false;
			if (notSet && instance.set == notSet) return false;

			return true;
		}
	};

	struct DiscFilter {
		std::array<DiscSlotFilter, 6> filters{
			DiscSlotFilter{.partition = Disc::Partition::one},
			DiscSlotFilter{.partition = Disc::Partition::two},
			DiscSlotFilter{.partition = Disc::Partition::three},
			DiscSlotFilter{.partition = Disc::Partition::four},
			DiscSlotFilter{.partition = Disc::Partition::five},
			DiscSlotFilter{.partition = Disc::Partition::six},
		};
		std::optional<Stats::DiscBonus> bonus1;
		std::optional<Stats::DiscBonus> bonus2;
		std::optional<Stats::DiscBonus> bonus3;

		FilteredDiscs filter(std::vector<Disc::Instance> &discs) const {
			FilteredDiscs ret{};
			for (auto [dst, filter]: std::views::zip(ret.entries, filters)) {
				for (auto &disc: discs) {
					if (filter.eval(disc)) dst.emplace_back(&disc);
				}
			}

			return ret;
		}
		FilteredDiscs filter(FilteredDiscs &discs) const {
			FilteredDiscs ret{};
			for (auto [discs, dst, filter]: std::views::zip(discs.entries, ret.entries, filters)) {
				for (auto &disc: discs) {
					if (filter.eval(*disc)) dst.emplace_back(disc);
				}
			}

			return ret;
		}
	};

	// Combines all the discs of a slot into one unrealistically optimistic one to get the max potential
	static inline std::array<Stats::Sheet<float>, 6> getMaxStatsForSlots(auto &&discs) {
		std::array<Stats::Sheet<float>, 6> statsForPartition{};

		for (auto &&[partition, discs]: std::views::zip(statsForPartition, discs.entries)) {
			for (const auto &disc: discs) {
				auto &mainStatSlot = partition.fromStat(disc->mainStat);
				auto &mainStatDisc = disc->stats.fromStat(disc->mainStat);
				mainStatSlot = std::max(mainStatSlot, mainStatDisc);

				for (const auto &subStat: disc->subStats) {
					if (!subStat.stat.has_value()) continue;
					auto &statSlot = partition.fromStat(subStat.stat.value());
					auto &statDisc = disc->stats.fromStat(subStat.stat.value());
					statSlot = std::max(statSlot, statDisc);
				}
			}
		}
		return statsForPartition;
	}
}// namespace Optimization