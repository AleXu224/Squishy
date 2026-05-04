#pragma once

#include "disc/instance.hpp"
#include "disc/key.hpp"
#include "disc/slot.hpp"
#include "ranges"
#include "stats/disc.hpp"
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