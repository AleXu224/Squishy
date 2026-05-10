#include "optimization.hpp"

Serialization::Save::Optimization Serialization::Save::Optimization::fromInstance(const ::Optimization::Options &options) {
	return Serialization::Save::Optimization{
		.nodeSource = [&]() -> std::optional<ComboSourceTypes> {
			if (!options.nodeSource.has_value()) return std::nullopt;
			return std::visit(
				Utils::overloaded{
					[](const ::Combo::Source::Agent &source) -> Serialization::Save::ComboSourceTypes {
						return Serialization::Save::AgentCombo{
							.key = source.key,
							.slot = source.slot,
							.index = source.index,
						};
					},
					[](const ::Combo::Source::Combo &source) -> Serialization::Save::ComboSourceTypes {
						return Serialization::Save::ComboCombo{
							.agentKey = source.agentKey,
							.comboKey = source.comboKey,
						};
					},
					[](const ::Combo::Source::Engine &source) -> Serialization::Save::ComboSourceTypes {
						return Serialization::Save::EngineCombo{
							.key = source.key,
							.index = source.index,
						};
					},
					[](const ::Combo::Source::Disc &source) -> Serialization::Save::ComboSourceTypes {
						return Serialization::Save::DiscCombo{
							.key = source.key,
							.slot = source.slot,
							.index = source.index,
						};
					},
					[](const ::Combo::Source::Anomaly &source) -> Serialization::Save::ComboSourceTypes {
						return Serialization::Save::AnomalyCombo{
							.reaction = source.reaction,
						};
					},
				},
				options.nodeSource.value()
			);
		}(),
		.twoPcSets = options.twoPcSets,
		.fourPcSets = options.fourPcSets,
		.partition4MainStats = options.partition4MainStats,
		.partition5MainStats = options.partition5MainStats,
		.partition6MainStats = options.partition6MainStats,
		.useEquippedDiscs = options.useEquippedDiscs,
	};
}

::Optimization::Options Serialization::Save::Optimization::toInstance() const {
	::Optimization::Options ret{};

	for (const auto &[key, value]: twoPcSets) {
		ret.twoPcSets[key] = value;
	}
	for (const auto &[key, value]: fourPcSets) {
		ret.fourPcSets[key] = value;
	}
	for (const auto &[key, value]: partition4MainStats) {
		ret.partition4MainStats[key] = value;
	}
	for (const auto &[key, value]: partition5MainStats) {
		ret.partition5MainStats[key] = value;
	}
	for (const auto &[key, value]: partition6MainStats) {
		ret.partition6MainStats[key] = value;
	}

	ret.useEquippedDiscs = useEquippedDiscs;

	if (nodeSource.has_value()) {
		ret.nodeSource = std::visit(//
			Utils::overloaded{
				[](const Serialization::Save::AgentCombo &source) -> ::Combo::Source::Types {
					return ::Combo::Source::Agent{
						.key = source.key,
						.slot = source.slot,
						.index = source.index,
					};
				},
				[](const Serialization::Save::ComboCombo &source) -> ::Combo::Source::Types {
					return ::Combo::Source::Combo{
						.agentKey = source.agentKey,
						.comboKey = source.comboKey,
					};
				},
				[](const Serialization::Save::EngineCombo &source) -> ::Combo::Source::Types {
					return ::Combo::Source::Engine{
						.key = source.key,
						.index = source.index,
					};
				},
				[](const Serialization::Save::DiscCombo &source) -> ::Combo::Source::Types {
					return ::Combo::Source::Disc{
						.key = source.key,
						.slot = source.slot,
						.index = source.index,
					};
				},
				[](const Serialization::Save::AnomalyCombo &source) -> ::Combo::Source::Types {
					return ::Combo::Source::Anomaly{
						.reaction = source.reaction,
					};
				},
			},
			nodeSource.value()
		);
	}

	return ret;
}
