#pragma once

#include "fmt/core.h"
#include "stats/loadout.hpp"

namespace Formula {
	struct CharacterLevel {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character level {}", eval(context));
		}

		[[nodiscard]] inline uint32_t eval(const Context &context) const {
			return context.target.character.sheet.level;
		}
	};

	struct CharacterConstellation {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character constellation {}", eval(context));
		}

		[[nodiscard]] inline uint32_t eval(const Context &context) const {
			return context.target.character.sheet.constellation;
		}
	};

	struct CharacterAscension {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character ascension {}", eval(context));
		}

		[[nodiscard]] inline uint32_t eval(const Context &context) const {
			return context.target.character.sheet.ascension;
		}
	};
}// namespace Formula