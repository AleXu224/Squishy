#pragma once

#include "fmt/core.h"
#include "option/option.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	template<class T>
	struct impl_IsActive {
		Utils::HashedString name;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			const auto options = [&]() {
				if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
					return &context.source.character.options;
				else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
					return &context.source.weapon.options;
				else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
					return &context.source.artifact.options;
			}();
			return ::Option::getBool(*options, name);
		}
	};

	template<class T>
	struct impl_GetFloat {
		Utils::HashedString name;
		float defaultValue = 0.f;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", name.str, eval(context));
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			const auto options = [&]() {
				if constexpr (std::is_same_v<T, Stats::CharacterSheet>)
					return &context.source.character.options;
				else if constexpr (std::is_same_v<T, Stats::WeaponSheet>)
					return &context.source.weapon.options;
				else if constexpr (std::is_same_v<T, Stats::ArtifactSheet>)
					return &context.source.artifact.options;
			}();
			auto cond = ::Option::getFloat(*options, name, defaultValue);
			return cond;
		}
	};
}// namespace Formula