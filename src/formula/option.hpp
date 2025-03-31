#pragma once

#include "fmt/core.h"
#include "option/option.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Formula {
	template<class T>
	struct impl_IsActive {
		Utils::HashedString name;

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{}", name.str);
		}

		[[nodiscard]] bool eval(const Context &context) const {
			const auto options = [&]() {
				if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
					return &context.source.character.options;
				else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
					return &context.source.weapon->options;
				else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
					return &context.source.artifact.options;
				else if constexpr (std::is_same_v<T, Stats::Team>)
					return &context.team.options;
			}();
			return ::Option::getBool(*options, name);
		}
	};

	template<class T>
	struct impl_GetFloat {
		Utils::HashedString name;
		float defaultValue = 0.f;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] float eval(const Context &context) const {
			const auto options = [&]() {
				if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
					return &context.source.character.options;
				else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
					return &context.source.weapon->options;
				else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
					return &context.source.artifact.options;
			}();
			return ::Option::getFloat(*options, name, defaultValue);
		}
	};

	template<class T>
	struct impl_GetInt {
		Utils::HashedString name;
		uint32_t defaultValue = 0.f;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] uint32_t eval(const Context &context) const {
			const auto options = [&]() {
				if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
					return &context.source.character.options;
				else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
					return &context.source.weapon->options;
				else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
					return &context.source.artifact.options;
			}();
			return ::Option::getInt(*options, name, defaultValue);
		}
	};
}// namespace Formula