#pragma once

#include "character/instance.hpp"
#include "fmt/core.h"
#include "formula/intermediary.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

namespace Formula {
	struct CharacterLevel {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character level {}", eval(context));
		}

		[[nodiscard]] inline uint32_t eval(const Context &context) const {
			return context.source.character.sheet.level;
		}
	};

	struct CharacterConstellation {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character constellation {}", eval(context));
		}

		[[nodiscard]] inline uint32_t eval(const Context &context) const {
			return context.source.character.sheet.constellation;
		}
	};

	struct CharacterAscension {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character ascension {}", eval(context));
		}

		[[nodiscard]] inline uint32_t eval(const Context &context) const {
			return context.source.character.sheet.ascension;
		}
	};


	template<FormulaLike T>
	struct MaxCharacter {
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			RetType ret{};
			Character::Instance *maxCharacter = nullptr;
			for (auto &character: context.team.characters) {
				if (!character) continue;
				auto val = formula.eval(context.withSource(character->loadout));
				if (val > ret) {
					ret = val;
					maxCharacter = character;
				}
			}

			if (!maxCharacter) return "";

			return fmt::format("Max Team {}", formula.print(context.withSource(maxCharacter->loadout), prevStep));
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			RetType ret{};
			for (const auto &character: context.team.characters) {
				if (!character) continue;
				ret = std::max(ret, formula.eval(context.withSource(character->loadout)));
			}
			return ret;
		}
	};
}// namespace Formula