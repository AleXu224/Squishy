#include "artifact.hpp"

#include "artifact/sets.hpp"
#include "ranges"
#include "store.hpp"
#include "utility"


void Stats::Artifact::Slotted::unequipAll() {
	for (const auto &member: getMembers()) {
		auto &slot = std::invoke(member, *this);

		if (!slot) continue;
		slot.clear();
	}
}

namespace {
	struct Occurence {
		::Artifact::SetKey set{};
		uint8_t count = 0;
	};
	struct OccurenceMapper {
		std::array<Occurence, 5> occurences{};

		Occurence &get(const ::Artifact::SetKey &key) {
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

Stats::Artifact::Slotted &Stats::Artifact::getSlotted() {
	return std::get<Stats::Artifact::Slotted>(equipped);
}

const Stats::Artifact::Slotted &Stats::Artifact::getSlotted() const {
	return std::get<Stats::Artifact::Slotted>(equipped);
}

Stats::Artifact::Theorycraft &Stats::Artifact::getTheorycraft() {
	return std::get<Stats::Artifact::Theorycraft>(equipped);
}

const Stats::Artifact::Theorycraft &Stats::Artifact::getTheorycraft() const {
	return std::get<Stats::Artifact::Theorycraft>(equipped);
}

bool Stats::Artifact::isTheorycraft() const {
	return std::holds_alternative<Stats::Artifact::Theorycraft>(equipped);
}

void Stats::Artifact::refreshStats() {
	OccurenceMapper mapper{};
	bonus1 = std::nullopt;
	bonus2 = std::nullopt;
	for (auto &artifact: sheet.equippedArtifacts) {
		artifact = std::nullopt;
	}

	std::visit(
		Utils::overloaded{
			[&](const Stats::Artifact::Slotted &slotted) {
				for (const auto &[artPtr, index]: std::views::zip(Stats::Artifact::Slotted::getMembers(), std::views::iota(0, 5))) {
					auto artId = std::invoke(artPtr, slotted);
					if (!artId) continue;
					auto &artifact = Store::artifacts.at(artId);
					mapper.get(artifact.set).count++;
					sheet.equippedArtifacts.at(index) = std::make_optional(&artifact.stats);
				}

				for (auto &occurence: mapper.occurences) {
					if (!occurence.set) break;
					if (occurence.count >= 2) {
						// Cache the ArtifactData to avoid doing two accesses
						const ::Artifact::Set &artifactData = ::Artifact::sets.at(occurence.set);

						auto &targetBonus = [&]() -> std::optional<Stats::ArtifactBonus> & {
							if (!bonus1.has_value()) return bonus1;

							return bonus2;
						}();
						targetBonus.emplace(Stats::ArtifactBonus{
							.setPtr = &artifactData,
							.bonusPtr = &artifactData.data.twoPc,
						});

						// The second check should not be be outside since a four set can only happen
						// only if there is a two set
						if (occurence.count >= 4) {
							bonus2.emplace(Stats::ArtifactBonus{
								.setPtr = &artifactData,
								.bonusPtr = &artifactData.data.fourPc,
							});
						}
					}
				}
			},
			[&](Stats::Artifact::Theorycraft &theorycraft) {
				if (theorycraft.set1.key) {
					const auto &set = ::Artifact::sets.at(theorycraft.set1.key);
					bonus1 = ArtifactBonus{
						.setPtr = &set,
						.bonusPtr = &set.data.twoPc,
					};
					if (theorycraft.set1.type == Theorycraft::Set::Type::fourPc) {
						bonus2 = ArtifactBonus{
							.setPtr = &set,
							.bonusPtr = &set.data.fourPc,
						};
					}
				}
				if (theorycraft.set2 && theorycraft.set1.key && theorycraft.set1.type == Theorycraft::Set::Type::twoPc) {
					const auto &set = ::Artifact::sets.at(theorycraft.set2);
					bonus2 = ArtifactBonus{
						.setPtr = &set,
						.bonusPtr = &set.data.twoPc,
					};
				}
				sheet.equippedArtifacts.at(0).emplace(&theorycraft.sheet);
			}
		},
		equipped
	);
}
