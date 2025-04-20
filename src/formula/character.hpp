#pragma once

#include "character/data.hpp"
#include "character/instance.hpp"
#include "compiled/constant.hpp"
#include "fmt/core.h"
#include "formula/intermediary.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"


namespace Formula {
	struct CharacterLevel {
		[[nodiscard]] inline Compiled::ConstantInt compile(const Context &context) const {
			return Compiled::ConstantInt{eval(context)};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character level {}", eval(context));
		}

		[[nodiscard]] inline uint32_t eval(const Context &context) const {
			return context.source.character.sheet.level;
		}
	};

	struct CharacterConstellation {
		[[nodiscard]] inline Compiled::ConstantInt compile(const Context &context) const {
			return Compiled::ConstantInt{eval(context)};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character constellation {}", eval(context));
		}

		[[nodiscard]] inline uint32_t eval(const Context &context) const {
			return context.source.character.sheet.constellation;
		}
	};

	struct CharacterAscension {
		[[nodiscard]] inline Compiled::ConstantInt compile(const Context &context) const {
			return Compiled::ConstantInt{eval(context)};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character ascension {}", eval(context));
		}

		[[nodiscard]] inline uint32_t eval(const Context &context) const {
			return context.source.character.sheet.ascension;
		}
	};

	struct IsCharacterWeaponType {
		Misc::WeaponType type;

		[[nodiscard]] inline Compiled::ConstantBool compile(const Context &context) const {
			return Compiled::ConstantBool{eval(context)};
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

		[[nodiscard]] inline Compiled::ConstantBool compile(const Context &context) const {
			return Compiled::ConstantBool{eval(context)};
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

		[[nodiscard]] inline Compiled::ConstantBool compile(const Context &context) const {
			return Compiled::ConstantBool{eval(context)};
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

		template<class V>
		struct Compiled {
			std::optional<V> formula1;
			std::optional<V> formula2;
			std::optional<V> formula3;
			std::optional<V> formula4;

			[[nodiscard]] auto eval(const Formula::Context &context) const {
				RetType ret{};
				if (formula1.has_value()) ret = std::max(ret, formula1->eval(context));
				if (formula2.has_value()) ret = std::max(ret, formula2->eval(context));
				if (formula3.has_value()) ret = std::max(ret, formula3->eval(context));
				if (formula4.has_value()) ret = std::max(ret, formula4->eval(context));
				return ret;
			}

			[[nodiscard]] bool isConstant() const {
				if (formula1.has_value() && !formula1->isConstant()) return false;
				if (formula2.has_value() && !formula2->isConstant()) return false;
				if (formula3.has_value() && !formula3->isConstant()) return false;
				if (formula4.has_value() && !formula4->isConstant()) return false;
				return true;
			}
		};

		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled{
				context.team.characters[0] ? std::make_optional(formula.compile(context.withSource(context.team.characters[0]->loadout))) : std::nullopt,
				context.team.characters[1] ? std::make_optional(formula.compile(context.withSource(context.team.characters[1]->loadout))) : std::nullopt,
				context.team.characters[2] ? std::make_optional(formula.compile(context.withSource(context.team.characters[2]->loadout))) : std::nullopt,
				context.team.characters[3] ? std::make_optional(formula.compile(context.withSource(context.team.characters[3]->loadout))) : std::nullopt,
			};
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