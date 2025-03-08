#include "artifact.hpp"

#include "ranges"
#include "store.hpp"


void Stats::Artifact::Slotted::unequipAll() {
	for (const auto &member: getMembers()) {
		auto &slot = std::invoke(member, *this);

		if (!slot.has_value()) continue;
		auto &artifact = ::Store::artifacts.at(slot.value());
		artifact.equippedCharacter.clear();
	}
}

void Stats::Artifact::refreshStats() {
	static std::unordered_map<::Artifact::SetKey, uint8_t> occurences{};
	bonus1 = std::nullopt;
	bonus2 = std::nullopt;
	for (auto &artifact: sheet.equippedArtifacts) {
		artifact = std::nullopt;
	}

	for (const auto &[artPtr, index]: std::views::zip(Stats::Artifact::Slotted::getMembers(), std::views::iota(0, 5))) {
		auto artId = std::invoke(artPtr, equipped);
		if (!artId.has_value()) continue;
		auto &artifact = Store::artifacts.at(artId.value());
		occurences[artifact.set]++;
		sheet.equippedArtifacts.at(index) = std::make_optional(&artifact.stats);
	}
	for (auto &occurence: occurences) {
		if (occurence.second >= 2) {
			// Cache the ArtifactData to avoid doing two accesses
			const ::Artifact::Set &artifactData = ::Artifact::sets.at(occurence.first);

			auto &targetBonus = [&]() -> std::optional<Stats::ArtifactBonus> & {
				if (!bonus1.has_value()) return bonus1;

				return bonus2;
			}();
			targetBonus.emplace(Stats::ArtifactBonus{
				.setPtr = artifactData,
				.bonusPtr = artifactData.data.twoPc,
			});

			// The second check should not be be outside since a four set can only happen
			// only if there is a two set
			if (occurence.second >= 4) {
				bonus2.emplace(Stats::ArtifactBonus{
					.setPtr = artifactData,
					.bonusPtr = artifactData.data.fourPc,
				});
			}
		}
	}
	occurences.clear();
}
