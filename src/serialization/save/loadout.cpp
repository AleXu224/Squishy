#include "loadout.hpp"

#include "stats/loadout.hpp"
#include "store.hpp"
#include "weapon/data.hpp"
#include "weapon/defaultWeapons.hpp"

namespace Serialization::Save {
	ArtifactSlotted ArtifactSlotted::fromInstance(const ::Stats::Artifact::Slotted &instance) {
		return {
			.flower = instance.flower,
			.plume = instance.plume,
			.sands = instance.sands,
			.goblet = instance.goblet,
			.circlet = instance.circlet,
		};
	}

	::Stats::Artifact::Slotted ArtifactSlotted::toInstance() const {
		return {
			.flower = flower,
			.plume = plume,
			.sands = sands,
			.goblet = goblet,
			.circlet = circlet,
		};
	}

	ArtifactTC ArtifactTC::fromInstance(const ::Stats::Artifact::Theorycraft &instance) {
		return {
			.set1 = instance.set1,
			.set2 = instance.set2,
			.mainStats = [&]() {
				std::array<MainStat, 5> ret;
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

	::Stats::Artifact::Theorycraft ArtifactTC::toInstance() const {
		::Stats::Artifact::Theorycraft ret{
			.set1 = set1,
			.set2 = set2,
			.mainStats = [&]() {
				std::array<::Stats::Artifact::Theorycraft::MainStat, 5> ret;
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
			.weaponInstanceKey = instance.weaponInstanceKey,
			.artifact = std::visit(
				Utils::overloaded{
					[](const ::Stats::Artifact::Slotted &slotted) -> Serialization::Save::ArtifactTypes {
						return ArtifactSlotted::fromInstance(slotted);
					},
					[](const ::Stats::Artifact::Theorycraft &theorycraft) -> Serialization::Save::ArtifactTypes {
						return ArtifactTC::fromInstance(theorycraft);
					},
				},
				instance.artifact.equipped
			),
		};
	}

	::Stats::Loadout Loadout::toInstance(Misc::WeaponType weaponType) const {
		using Types = std::variant<::Stats::Artifact::Slotted, ::Stats::Artifact::Theorycraft>;
		auto usedWeaponInstanceKey = weaponInstanceKey;
		if (!usedWeaponInstanceKey || ::Store::weapons.at(usedWeaponInstanceKey).stats.data->baseStats.type != weaponType) {
			usedWeaponInstanceKey = ::Store::createWeapon(Weapon::defaultWeapons.at(weaponType)).instanceKey;
		}
		::Stats::Loadout ret{
			.weaponInstanceKey = usedWeaponInstanceKey,
			.weapon = &::Store::weapons.at(usedWeaponInstanceKey).stats,
			.artifact{
				.equipped = std::visit(
					Utils::overloaded{
						[](const ArtifactSlotted &slotted) -> Types {
							return slotted.toInstance();
						},
						[](const ArtifactTC &theorycraft) -> Types {
							return theorycraft.toInstance();
						},
					},
					artifact
				),
			},
		};
		return ret;
	}
}// namespace Serialization::Save