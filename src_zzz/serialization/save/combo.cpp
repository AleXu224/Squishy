#include "combo.hpp"

std::vector<Serialization::Save::Combo> Serialization::Save::comboFromInstance(const std::unordered_map<::Combo::InstanceKey, ::Combo::Combo> &combos) {
	std::vector<Serialization::Save::Combo> ret;

	for (const auto &[key, combo]: combos) {
		std::vector<Serialization::Save::ComboEntryTypes> comboEntries;

		for (const auto &entry: combo.entries) {
			std::visit(
				Utils::overloaded{
					[&](const ::Combo::Entry &entry) {
						comboEntries.emplace_back(Serialization::Save::ComboEntry{
							.multiplier = entry.multiplier,
							.source = [&]() -> Serialization::Save::ComboSourceTypes {
								Serialization::Save::ComboSourceTypes ret;

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
									entry.source
								);

								return ret;
							}(),
							.options = [&]() {
								std::vector<ComboOption> ret;

								for (const auto &opt: entry.options) {
									ret.emplace_back(ComboOption{
										.key = opt.key,
										.hash = opt.hash,
										.value = std::visit(//
											Utils::overloaded{
												[](const bool &val) -> decltype(ComboOption::value) {
													return ComboOptionBool{val};
												},
												[](const std::optional<uint8_t> &val) -> decltype(ComboOption::value) {
													return ComboOptionValueList{val};
												},
											},
											opt.value
										),
									});
								}

								return ret;
							}(),
						});
					},
					[&](const ::Combo::StateChangeEntry &entry) {
						comboEntries.emplace_back(Serialization::Save::ComboStateChangeEntry{
							.options = [&]() {
								std::vector<ComboOption> ret;

								for (const auto &opt: entry.options) {
									ret.emplace_back(ComboOption{
										.key = opt.key,
										.hash = opt.hash,
										.value = std::visit(//
											Utils::overloaded{
												[](const bool &val) -> decltype(ComboOption::value) {
													return ComboOptionBool{val};
												},
												[](const std::optional<uint8_t> &val) -> decltype(ComboOption::value) {
													return ComboOptionValueList{val};
												},
											},
											opt.value
										),
									});
								}

								return ret;
							}(),
						});
					},
				},
				entry
			);
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
		std::list<::Combo::EntryTypes> entries;

		for (const auto &entry: combo.entries) {
			std::visit(
				Utils::overloaded{
					[&](const Serialization::Save::ComboEntry &entry) {
						entries.emplace_back(::Combo::Entry{
							.multiplier = entry.multiplier,
							.source = std::visit(//
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
								entry.source
							),
							.options = [&]() {
								std::vector<::Combo::Option> ret;

								for (const auto &opt: entry.options) {
									ret.emplace_back(::Combo::Option{
										.key = opt.key,
										.hash = opt.hash,
										.value = std::visit(//
											Utils::overloaded{
												[](const ComboOptionBool &val) -> decltype(::Combo::Option::value) {
													return val.value;
												},
												[](const ComboOptionValueList &val) -> decltype(::Combo::Option::value) {
													return val.value;
												},
											},
											opt.value
										),
									});
								}

								return ret;
							}(),
						});
					},
					[&](const Serialization::Save::ComboStateChangeEntry &entry) {
						entries.emplace_back(::Combo::StateChangeEntry{
							.options = [&]() {
								std::vector<::Combo::Option> ret;

								for (const auto &opt: entry.options) {
									ret.emplace_back(::Combo::Option{
										.key = opt.key,
										.hash = opt.hash,
										.value = std::visit(//
											Utils::overloaded{
												[](const ComboOptionBool &val) -> decltype(::Combo::Option::value) {
													return val.value;
												},
												[](const ComboOptionValueList &val) -> decltype(::Combo::Option::value) {
													return val.value;
												},
											},
											opt.value
										),
									});
								}

								return ret;
							}(),
						});
					},
				},
				entry
			);
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
