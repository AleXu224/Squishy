#pragma once

#include "option/option.hpp"
#include <optional>
#include <variant>
#include <vector>

#include "glaze/glaze.hpp"// IWYU pragma: keep


namespace Serialization::Save {
	struct BooleanOption {
		uint32_t hash;
		bool active;

		auto operator<=>(const BooleanOption &) const = default;
	};

	struct ValueListOption {
		uint32_t hash{};
		std::optional<size_t> index;

		auto operator<=>(const ValueListOption &) const = default;
	};

	using OptionTypes = std::variant<Serialization::Save::BooleanOption, Serialization::Save::ValueListOption>;

	std::vector<OptionTypes> optionsFromInstance(const Option::TypesMap &options);
	void optionsToInstance(const std::vector<OptionTypes> &options, Option::TypesMap &target);
}// namespace Serialization::Save

template<>
struct glz::meta<Serialization::Save::BooleanOption> {
	using T = Serialization::Save::BooleanOption;
	static constexpr auto value = object(&T::hash, &T::active);
};
template<>
struct glz::meta<Serialization::Save::ValueListOption> {
	using T = Serialization::Save::ValueListOption;
	static constexpr auto value = object(&T::hash, &T::index);
};
template<>
struct glz::meta<Serialization::Save::OptionTypes> {
	static constexpr std::string_view tag = "type";
};