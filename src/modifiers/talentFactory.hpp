#pragma once

#include "type_traits"
#include "helpers.hpp"

namespace Modifiers {
	template<class T>
	struct TalentPointerFactory {
		using TT = std::remove_cvref_t<T>;

		static constexpr auto normal = &TT::normal;
		static constexpr auto skill = &TT::skill;
		static constexpr auto burst = &TT::burst;
	};

	struct TalentNameFactory {
		static constexpr auto normal = Modifiers::SheetMemberIdentifier(LevelableTalent::normal);
		static constexpr auto skill = Modifiers::SheetMemberIdentifier(LevelableTalent::skill);
		static constexpr auto burst = Modifiers::SheetMemberIdentifier(LevelableTalent::burst);
	};

	template<template<auto...> class Formula, auto... Params>
	struct TalentFactory {
		static constexpr Formula<Params.normal...> normal{};
		static constexpr Formula<Params.skill...> skill{};
		static constexpr Formula<Params.burst...> burst{};
	};
}// namespace Modifiers