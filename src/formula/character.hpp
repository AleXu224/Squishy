#pragma once

#include "character/data.hpp"
#include "character/instance.hpp"
#include "formula/base.hpp"
#include "formula/clamp.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"


namespace Formula {
	struct CharacterLevel : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character level {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.stats.sheet.level;
		}
	};

	struct CharacterConstellation : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character constellation {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.stats.sheet.constellation;
		}
	};

	struct CharacterAscension : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character ascension {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.stats.sheet.ascension;
		}
	};

	struct CharacterMoonsignLevel : FormulaBase<int32_t, Type::constant> {
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("Character Moonsign Level {}", eval(context));
		}

		[[nodiscard]] inline int32_t eval(const Context &context) const {
			return context.source.stats.sheet.moonsignLevel.eval(context);
		}
	};

	struct IsCharacterWeaponType : FormulaBase<bool, Type::constant> {
		Misc::WeaponType type;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character {} ({})", Utils::Stringify(type), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.stats.base.weaponType == type;
		}
	};

	struct IsTargetCharacterWeaponType : FormulaBase<bool, Type::constant> {
		Misc::WeaponType type;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is target character {} ({})", Utils::Stringify(type), eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.prevSource.stats.base.weaponType == type;
		}
	};

	struct IsCharacterId : FormulaBase<bool, Type::constant> {
		uint32_t id;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character id {} ({})", id, eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.source.stats.data.key.key == id;
		}
	};

	struct IsActiveCharacterId : FormulaBase<bool, Type::constant> {
		uint32_t id;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Is character id {} ({})", id, eval(context));
		}

		[[nodiscard]] bool eval(const Context &context) const {
			return context.active.stats.data.key.key == id;
		}
	};


	template<FormulaLike T>
	struct MaxCharacter : FormulaBase<FormulaType<T>> {
		T formula;

		using RetType = FormulaType<T>;

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			NodeType<RetType> ret = ConstantBase<RetType>{};

			for (const auto &character: ctx.team.characters) {
				if (!character) continue;
				ret = Max{.val1 = ret, .val2 = formula.fold(ctx.withSource(character->state), args)};
			}

			return ret.fold(ctx, args);
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
