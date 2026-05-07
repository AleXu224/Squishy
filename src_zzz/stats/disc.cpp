#include "disc.hpp"

#include "disc/sets.hpp"
#include "ranges"
#include "store.hpp"
#include "utility"


void Stats::Disc::Slotted::unequipAll() {
	for (const auto &member: getMembers()) {
		auto &slot = std::invoke(member, *this);

		if (!slot) continue;
		slot.clear();
	}
}

namespace {
	struct Occurence {
		::Disc::SetKey set{};
		uint8_t count = 0;
	};
	struct OccurenceMapper {
		std::array<Occurence, 6> occurences{};

		Occurence &get(const ::Disc::SetKey &key) {
			for (auto &occurence: occurences) {
				if (!occurence.set) {
					occurence.set = key;
					return occurence;
				}
				if (occurence.set == key) return occurence;
			}
			std::unreachable();
		}
	};
}// namespace

Stats::Disc::Slotted &Stats::Disc::getSlotted() {
	return std::get<Stats::Disc::Slotted>(equipped);
}

const Stats::Disc::Slotted &Stats::Disc::getSlotted() const {
	return std::get<Stats::Disc::Slotted>(equipped);
}

Stats::Disc::Theorycraft &Stats::Disc::getTheorycraft() {
	return std::get<Stats::Disc::Theorycraft>(equipped);
}

const Stats::Disc::Theorycraft &Stats::Disc::getTheorycraft() const {
	return std::get<Stats::Disc::Theorycraft>(equipped);
}

bool Stats::Disc::isTheorycraft() const {
	return std::holds_alternative<Stats::Disc::Theorycraft>(equipped);
}

void Stats::Disc::refreshStats() {
	OccurenceMapper mapper{};
	bonus1 = std::nullopt;
	bonus2 = std::nullopt;
	bonus3 = std::nullopt;
	for (auto &disc: sheet.equippedDiscs) {
		disc = std::nullopt;
	}

	std::visit(
		Utils::overloaded{
			[&](const Stats::Disc::Slotted &slotted) {
				for (const auto &[artPtr, index]: std::views::zip(Stats::Disc::Slotted::getMembers(), std::views::iota(0, 6))) {
					auto artId = std::invoke(artPtr, slotted);
					if (!artId) continue;
					auto &disc = Store::discs.at(artId);
					mapper.get(disc.set).count++;
					sheet.equippedDiscs.at(index) = std::make_optional(&disc.stats);
				}

				constexpr auto bonuses = std::array{
					&Stats::Disc::bonus1,
					&Stats::Disc::bonus2,
					&Stats::Disc::bonus3,
				};
				uint32_t bonusIndex = 0;

				for (auto &occurence: mapper.occurences) {
					if (!occurence.set) break;
					if (occurence.count >= 2) {
						// Cache the DiscData to avoid doing two accesses
						const ::Disc::Set &discData = ::Disc::sets.at(occurence.set);

						std::invoke(bonuses[bonusIndex++], *this).emplace(Stats::DiscBonus{
							.setPtr = &discData,
							.bonusPtr = &discData.data.twoPc,
						});

						// The second check should not be be outside since a four set can only happen
						// only if there is a two set
						if (occurence.count >= 4) {
							std::invoke(bonuses[bonusIndex++], *this).emplace(Stats::DiscBonus{
								.setPtr = &discData,
								.bonusPtr = &discData.data.fourPc,
							});
						}
					}
				}
			},
			[&](Stats::Disc::Theorycraft &theorycraft) {
				if (theorycraft.set1.key) {
					const auto &set = ::Disc::sets.at(theorycraft.set1.key);
					bonus1 = DiscBonus{
						.setPtr = &set,
						.bonusPtr = &set.data.twoPc,
					};
					if (theorycraft.set1.type == Theorycraft::Set::Type::fourPc) {
						bonus2 = DiscBonus{
							.setPtr = &set,
							.bonusPtr = &set.data.fourPc,
						};
					}
				}
				if (theorycraft.set2) {
					const auto &set = ::Disc::sets.at(theorycraft.set2);
					bonus2 = DiscBonus{
						.setPtr = &set,
						.bonusPtr = &set.data.twoPc,
					};
				}
				if (theorycraft.set3 && theorycraft.set1.key && theorycraft.set1.type == Theorycraft::Set::Type::twoPc) {
					const auto &set = ::Disc::sets.at(theorycraft.set3);
					bonus3 = DiscBonus{
						.setPtr = &set,
						.bonusPtr = &set.data.twoPc,
					};
				}
				sheet.equippedDiscs.at(0).emplace(&theorycraft.sheet);
			}
		},
		equipped
	);
}
