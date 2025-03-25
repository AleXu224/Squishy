#include "combo.hpp"

std::vector<Serialization::Save::Combo> Serialization::Save::comboFromInstance(const std::unordered_map<::Combo::InstanceKey, ::Combo::Combo> &combos) {
	std::vector<Serialization::Save::Combo> ret;

	for (const auto &[key, combo]: combos) {
		std::vector<Serialization::Save::ComboEntry> comboEntries;

		for (const auto &entry: combo.entries) {
			comboEntries.emplace_back(Serialization::Save::ComboEntry{
				.multiplier = entry.multiplier,
				.reaction = entry.reaction,
				.source = [&]() -> Serialization::Save::ComboSourceTypes {
					Serialization::Save::ComboSourceTypes ret;

					return std::visit(
						Utils::overloaded{
							[](const ::Combo::Source::Character &source) -> Serialization::Save::ComboSourceTypes {
								return Serialization::Save::CharacterCombo{
									.key = source.key,
									.slot = source.slot,
									.index = source.index,
								};
							},
							[](const ::Combo::Source::Combo &source) -> Serialization::Save::ComboSourceTypes {
								return Serialization::Save::ComboCombo{
									.characterKey = source.characterKey,
									.comboKey = source.comboKey,
								};
							},
							[](const ::Combo::Source::Weapon &source) -> Serialization::Save::ComboSourceTypes {
								return Serialization::Save::WeaponCombo{
									.key = source.key,
									.index = source.index,
								};
							},
							[](const ::Combo::Source::Artifact &source) -> Serialization::Save::ComboSourceTypes {
								return Serialization::Save::ArtifactCombo{
									.key = source.key,
									.slot = source.slot,
									.index = source.index,
								};
							},
						},
						entry.source
					);

					return ret;
				}(),
			});
		}

		ret.emplace_back(Serialization::Save::Combo{
			.instanceKey = key,
			.name = combo.name,
			.entries = comboEntries,
		});
	}

	return ret;
}

std::unordered_map<::Combo::InstanceKey, ::Combo::Combo> Serialization::Save::comboToInstance(const std::vector<Serialization::Save::Combo> &combos) {
	std::unordered_map<::Combo::InstanceKey, ::Combo::Combo> ret;

	for (const auto &combo: combos) {
		std::list<::Combo::Entry> entries;

		for (const auto &entry: combo.entries) {
			entries.emplace_back(::Combo::Entry{
				.multiplier = entry.multiplier,
				.reaction = entry.reaction,
				.source = std::visit(Utils::overloaded{
										 [](const Serialization::Save::CharacterCombo &source) -> ::Combo::Source::Types {
											 return ::Combo::Source::Character{
												 .key = source.key,
												 .slot = source.slot,
												 .index = source.index,
											 };
										 },
										 [](const Serialization::Save::ComboCombo &source) -> ::Combo::Source::Types {
											 return ::Combo::Source::Combo{
												 .characterKey = source.characterKey,
												 .comboKey = source.comboKey,
											 };
										 },
										 [](const Serialization::Save::WeaponCombo &source) -> ::Combo::Source::Types {
											 return ::Combo::Source::Weapon{
												 .key = source.key,
												 .index = source.index,
											 };
										 },
										 [](const Serialization::Save::ArtifactCombo &source) -> ::Combo::Source::Types {
											 return ::Combo::Source::Artifact{
												 .key = source.key,
												 .slot = source.slot,
												 .index = source.index,
											 };
										 },
									 },
									 entry.source),
			});
		}

		ret.insert({
			combo.instanceKey,
			::Combo::Combo{
				.instanceKey = combo.instanceKey,
				.name = combo.name,
				.entries = entries,
			},
		});
	}

	return ret;
}
