#pragma once

#include "character/data.hpp"
#include "character/instance.hpp"
#include "compiled/constant.hpp"
#include "compiled/max.hpp"
#include "fmt/core.h"
#include "formula/intermediary.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"


namespace Formula {
	struct CharacterLevel {
		[[nodiscard]] inline Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character level {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.character.sheet.level;
		}
	};

	struct CharacterConstellation {
		[[nodiscard]] inline Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character constellation {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.character.sheet.constellation;
		}
	};

	struct CharacterAscension {
		[[nodiscard]] inline Compiled::IntNode compile(const Context &context) const {
			return Compiled::ConstantInt{.value = eval(context)};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character ascension {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.character.sheet.ascension;
		}
	};

	struct IsCharacterWeaponType {
		Misc::WeaponType type;

		[[nodiscard]] inline Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character {} ({})", Utils::Stringify(type), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.character.base.weaponType == type;
		}
	};

	struct IsCharacterId {
		uint32_t id;

		[[nodiscard]] inline Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character id {} ({})", id, eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.character.data.key.key == id;
		}
	};

	struct IsActiveCharacterId {
		uint32_t id;

		[[nodiscard]] inline Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character id {} ({})", id, eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.active.character.data.key.key == id;
		}
	};


	template<FormulaLike T>
	struct MaxCharacter {
		T formula;

		using RetType = FormulaType<T>;

		[[nodiscard]] inline auto compile(const Context &context) const {
			Compiled::NodeType<RetType> ret = Compiled::Constant<RetType>{};

			for (const auto &character: context.team.characters) {
				if (!character) continue;
				ret = Compiled::Max{.val1 = ret, .val2 = formula.compile(context.withSource(character->loadout))}.wrap();
			}

			return ret;
		}

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