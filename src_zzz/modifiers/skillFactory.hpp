#pragma once

#include "helpers.hpp"
#include "type_traits"


namespace Modifiers {
	template<class T>
	struct SkillPointerFactory {
		using TT = std::remove_cvref_t<T>;

		static constexpr auto basic = &TT::basic;
		static constexpr auto dodge = &TT::dodge;
		static constexpr auto assist = &TT::assist;
		static constexpr auto special = &TT::special;
		static constexpr auto chain = &TT::chain;
		static constexpr auto core = &TT::core;
	};

	struct SkillNameFactory {
		static constexpr auto basic = Modifiers::SheetMemberIdentifier(LevelableSkill::basic);
		static constexpr auto dodge = Modifiers::SheetMemberIdentifier(LevelableSkill::dodge);
		static constexpr auto assist = Modifiers::SheetMemberIdentifier(LevelableSkill::assist);
		static constexpr auto special = Modifiers::SheetMemberIdentifier(LevelableSkill::special);
		static constexpr auto chain = Modifiers::SheetMemberIdentifier(LevelableSkill::chain);
		static constexpr auto core = Modifiers::SheetMemberIdentifier(LevelableSkill::core);
	};

	template<template<auto...> class Formula, auto... Params>
	struct SkillFactory {
		static constexpr Formula<Params.basic...> basic{};
		static constexpr Formula<Params.dodge...> dodge{};
		static constexpr Formula<Params.assist...> assist{};
		static constexpr Formula<Params.special...> special{};
		static constexpr Formula<Params.chain...> chain{};
		static constexpr Formula<Params.core...> core{};
	};

	template<class T, class Formula>
	[[nodiscard]] inline Skills<T> skillFactory(auto... params) {
		return {
			.basic = Formula({}, params.basic...),
			.dodge = Formula({}, params.dodge...),
			.assist = Formula({}, params.assist...),
			.special = Formula({}, params.special...),
			.chain = Formula({}, params.chain...),
			.core = Formula({}, params.core...),
		};
	}
}// namespace Modifiers