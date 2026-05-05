#pragma once

#include "misc/anomaly.hpp"
#include "variant"


#include "agent/key.hpp"
#include "combo/combo.hpp"
#include "disc/key.hpp"
#include "disc/slot.hpp"
#include "engine/key.hpp"
#include "node/entry.hpp"


#include "glaze/glaze.hpp"// IWYU pragma: keep


namespace Serialization::Save {
	struct AgentCombo {
		::Agent::DataKey key;
		Node::AgentSlot slot;
		size_t index;
	};

	struct ComboCombo {
		::Agent::InstanceKey agentKey;
		::Combo::InstanceKey comboKey;
	};

	struct EngineCombo {
		::Engine::DataKey key;
		size_t index;
	};

	struct DiscCombo {
		::Disc::SetKey key;
		::Disc::SetSlot slot;
		size_t index;
	};

	struct AnomalyCombo {
		Misc::Anomaly reaction;
	};

	using ComboSourceTypes = std::variant<AgentCombo, ComboCombo, EngineCombo, DiscCombo, AnomalyCombo>;

	struct ComboOptionBool {
		bool value;
	};

	struct ComboOptionValueList {
		std::optional<uint8_t> value;
	};

	using ComboOptionTypes = std::variant<ComboOptionBool, ComboOptionValueList>;

	struct ComboOption {
		Agent::InstanceKey key;
		uint32_t hash;
		ComboOptionTypes value;
	};

	struct ComboEntry {
		float multiplier;
		ComboSourceTypes source;
		std::vector<ComboOption> options;

		bool operator==(auto &&other) const {
			return false;
		}

		bool operator<(auto &&other) const {
			return false;
		}
	};

	struct ComboStateChangeEntry {
		std::vector<ComboOption> options;

		bool operator==(auto &&other) const {
			return false;
		}

		bool operator<(auto &&other) const {
			return false;
		}
	};

	using ComboEntryTypes = std::variant<ComboEntry, ComboStateChangeEntry>;

	struct Combo {
		::Combo::InstanceKey instanceKey;
		std::string name;
		std::vector<ComboEntryTypes> entries;
	};

	std::vector<Serialization::Save::Combo> comboFromInstance(const std::unordered_map<::Combo::InstanceKey, ::Combo::Combo> &combos);
	std::unordered_map<::Combo::InstanceKey, ::Combo::Combo> comboToInstance(const std::vector<Serialization::Save::Combo> &combos);
}// namespace Serialization::Save
template<>
struct glz::meta<Serialization::Save::AgentCombo> {
	using T = Serialization::Save::AgentCombo;
	static constexpr auto value = object(&T::key, &T::slot, &T::index);
};
template<>
struct glz::meta<Serialization::Save::ComboCombo> {
	using T = Serialization::Save::ComboCombo;
	static constexpr auto value = object(&T::agentKey, &T::comboKey);
};
template<>
struct glz::meta<Serialization::Save::EngineCombo> {
	using T = Serialization::Save::EngineCombo;
	static constexpr auto value = object(&T::key, &T::index);
};
template<>
struct glz::meta<Serialization::Save::DiscCombo> {
	using T = Serialization::Save::DiscCombo;
	static constexpr auto value = object(&T::key, &T::slot, &T::index);
};
template<>
struct glz::meta<Serialization::Save::AnomalyCombo> {
	using T = Serialization::Save::AnomalyCombo;
	static constexpr auto value = object(&T::reaction);
};
template<>
struct glz::meta<Serialization::Save::ComboSourceTypes> {
	static constexpr std::string_view tag = "type";
};

template<>
struct glz::meta<Serialization::Save::ComboOptionBool> {
	using T = Serialization::Save::ComboOptionBool;
	static constexpr auto value = object(&T::value);
};
template<>
struct glz::meta<Serialization::Save::ComboOptionValueList> {
	using T = Serialization::Save::ComboOptionValueList;
	static constexpr auto value = object(&T::value);
};
template<>
struct glz::meta<Serialization::Save::ComboOptionTypes> {
	static constexpr std::string_view tag = "type";
};

template<>
struct glz::meta<Serialization::Save::ComboEntry> {
	using T = Serialization::Save::ComboEntry;
	static constexpr auto value = object(&T::multiplier, &T::source, &T::options);
};
template<>
struct glz::meta<Serialization::Save::ComboStateChangeEntry> {
	using T = Serialization::Save::ComboStateChangeEntry;
	static constexpr auto value = object(&T::options);
};
template<>
struct glz::meta<Serialization::Save::ComboEntryTypes> {
	static constexpr std::string_view tag = "type";
};