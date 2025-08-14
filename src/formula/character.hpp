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
			return context.source.stats.sheet.level;
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
			return context.source.stats.sheet.constellation;
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
			return context.source.stats.sheet.ascension;
		}
	};

	struct CharacterMoonsignLevel {
		[[nodiscard]] inline Compiled::IntNode compile(const Context &context) const {
			return context.source.stats.sheet.moonsignLevel.compile(context);
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character Moonsign Level {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.stats.sheet.moonsignLevel.eval(context);
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
			return context.source.stats.base.weaponType == type;
		}
	};

	struct IsActiveCharacterWeaponType {
		Misc::WeaponType type;

		[[nodiscard]] inline Compiled::BoolNode compile(const Context &context) const {
			return Compiled::ConstantBool{.value = eval(context)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is active character {} ({})", Utils::Stringify(type), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.active.stats.base.weaponType == type;
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
			return context.source.stats.data.key.key == id;
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
			return context.active.stats.data.key.key == id;
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
				ret = Compiled::Max{.val1 = ret, .val2 = formula.compile(context.withSource(character->state))}.wrap();
			}

			return ret;
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			RetType ret{};
			Character::Instance *maxCharacter = nullptr;
			for (auto &character: context.team.characters) {
				if (!character) continue;
				auto val = formula.eval(context.withSource(character->state));
				if (val > ret) {
					ret = val;
					maxCharacter = character;
				}
			}

			if (!maxCharacter) return "";

			return fmt::format("Max Team {}", formula.print(context.withSource(maxCharacter->state), prevStep));
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			RetType ret{};
			for (const auto &character: context.team.characters) {
				if (!character) continue;
				ret = std::max(ret, formula.eval(context.withSource(character->state)));
			}
			return ret;
		}
	};
}// namespace Formula
