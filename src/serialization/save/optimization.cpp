#include "optimization.hpp"

Serialization::Save::Optimization Serialization::Save::Optimization::fromInstance(const ::Optimization::Options &options) {
	return Serialization::Save::Optimization{
		.nodeSource = [&]() -> std::optional<ComboSourceTypes> {
			if (!options.nodeSource.has_value()) return std::nullopt;
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
					[](const ::Combo::Source::TransformativeReaction &source) -> Serialization::Save::ComboSourceTypes {
						return Serialization::Save::TransformativeReactionCombo{
							.reaction = source.reaction,
						};
					},
				},
				options.nodeSource.value()
			);
		}(),
		.twoPcSets = options.twoPcSets,
		.fourPcSets = options.fourPcSets,
		.threeRainbow = options.threeRainbow,
		.fiveRainbow = options.fiveRainbow,
	};
}

::Optimization::Options Serialization::Save::Optimization::toInstance() const {
	::Optimization::Options ret{};

	ret.threeRainbow = threeRainbow;
	ret.fiveRainbow = fiveRainbow;

	for (const auto &[key, value]: twoPcSets) {
		ret.twoPcSets[key] = value;
	}
	for (const auto &[key, value]: fourPcSets) {
		ret.fourPcSets[key] = value;
	}


	if (nodeSource.has_value()) {
		ret.nodeSource = std::visit(//
			Utils::overloaded{
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
				[](const Serialization::Save::TransformativeReactionCombo &source) -> ::Combo::Source::Types {
					return ::Combo::Source::TransformativeReaction{
						.reaction = source.reaction,
					};
				},
			},
			nodeSource.value()
		);
	}

	return ret;
}
