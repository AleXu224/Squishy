#include "loadout.hpp"

#include "engine/data.hpp"
#include "stats/loadout.hpp"
#include "store.hpp"


namespace Serialization::Save {
	DiscSlotted DiscSlotted::fromInstance(const ::Stats::Disc::Slotted &instance) {
		return {
			.one = instance.one,
			.two = instance.two,
			.three = instance.three,
			.four = instance.four,
			.five = instance.five,
			.six = instance.six,
		};
	}

	::Stats::Disc::Slotted DiscSlotted::toInstance() const {
		return {
			.one = one,
			.two = two,
			.three = three,
			.four = four,
			.five = five,
			.six = six,
		};
	}

	DiscTC DiscTC::fromInstance(const ::Stats::Disc::Theorycraft &instance) {
		return {
			.set1 = instance.set1,
			.set2 = instance.set2,
			.mainStats = [&]() {
				std::array<MainStat, 6> ret;
				for (const auto &[val1, val2]: std::views::zip(ret, instance.mainStats)) {
					val1 = {
						.stat = val2.stat,
						.level = val2.level,
						.rarity = val2.rarity,
					};
				}
				return ret;
			}(),
			.subStats = [&]() {
				std::vector<RollCount> ret;
				ret.reserve(::Stats::subStats.size());
				for (const auto &stat: ::Stats::subStats) {
					ret.emplace_back(RollCount{
						.stat = stat,
						.count = instance.fromStat(stat),
					});
				}
				return ret;
			}(),
		};
	}

	::Stats::Disc::Theorycraft DiscTC::toInstance() const {
		::Stats::Disc::Theorycraft ret{
			.set1 = set1,
			.set2 = set2,
			.mainStats = [&]() {
				std::array<::Stats::Disc::Theorycraft::MainStat, 6> ret;
				for (const auto &[val1, val2]: std::views::zip(ret, mainStats)) {
					val1 = {
						.stat = val2.stat,
						.level = val2.level,
						.rarity = val2.rarity,
					};
				}
				return ret;
			}(),
		};

		for (const auto &stat: subStats) {
			ret.fromStat(stat.stat) = stat.count;
		}

		ret.updateStats();

		return ret;
	}

	Loadout Loadout::fromInstance(const ::Stats::Loadout &instance) {
		return {
			.engineInstanceKey = instance.engineInstanceKey,
			.disc = std::visit(
				Utils::overloaded{
					[](const ::Stats::Disc::Slotted &slotted) -> Serialization::Save::DiscTypes {
						return DiscSlotted::fromInstance(slotted);
					},
					[](const ::Stats::Disc::Theorycraft &theorycraft) -> Serialization::Save::DiscTypes {
						return DiscTC::fromInstance(theorycraft);
					},
				},
				instance.disc.equipped
			),
		};
	}

	::Stats::Loadout Loadout::toInstance(Misc::Specialty specialty) const {
		using Types = std::variant<::Stats::Disc::Slotted, ::Stats::Disc::Theorycraft>;
		auto usedEngineInstanceKey = engineInstanceKey;
		if (!usedEngineInstanceKey || ::Store::engines.at(usedEngineInstanceKey).stats.data->baseStats.specialty != specialty) {
			usedEngineInstanceKey.key = 0;
		}
		::Stats::Loadout ret{
			.engineInstanceKey = usedEngineInstanceKey,
			.engine = &::Store::engines.at(usedEngineInstanceKey).stats,
			.disc{
				.equipped = std::visit(
					Utils::overloaded{
						[](const DiscSlotted &slotted) -> Types {
							return slotted.toInstance();
						},
						[](const DiscTC &theorycraft) -> Types {
							return theorycraft.toInstance();
						},
					},
					disc
				),
			},
		};
		return ret;
	}
}// namespace Serialization::Save